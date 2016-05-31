#include "AssimpCodec.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "ResourceDef.h"
SWORD_BEGIN
//AI_CONFIG_PP_SLM_VERTEX_LIMIT

class AssimpCodecImp {
public:
	void decode(const std::string& filename) {
		importer_.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);
	}

	void load(std::vector<Mesh>& mesh,
			  std::vector<Material>& material, Skeleton& skeleton, bool& has_skeleton) {
		const aiScene* scene = importer_.GetScene();

		has_skeleton = scene->HasAnimations();
		if (has_skeleton) {
			skeleton.buildJointTree(scene->mRootNode);
		}
		else {
			loadPreTransform(scene->mRootNode);
		}

		for (uint32_t i = 0; i < scene->mNumMeshes; ++i) {
			const aiMesh* ms = scene->mMeshes[i];
			if (ms->mFaces[0].mNumIndices != 3) continue;
			mesh.push_back(SWORD::Mesh());
			loadIndexInfo(ms, mesh.back());
			loadVertexInfo(ms, mesh.back(),i);
			loadJoint(ms, mesh.back(), skeleton);
			mesh.back().material_id = std::to_string(ms->mMaterialIndex);
		}

		assert(scene->HasMaterials());
		material.resize(scene->mNumMaterials);
		for (uint32_t i = 0; i < scene->mNumMaterials; i++) {
			loadMaterial(scene->mMaterials[i], material[i]);
		}

		assert((scene->HasAnimations() && scene->mNumAnimations != 0)
			   || (!scene->HasAnimations() && scene->mNumAnimations == 0));

		for (uint32_t i = 0; i < scene->mNumAnimations; ++i) {
			loadAnimation(scene->mAnimations[i], skeleton);
		}

		// todo
	}

private:
	glm::mat4 convert(aiMatrix4x4& m) {
		glm::mat4 r;
		for (int iRow = 0; iRow != 4; ++iRow)
			for (int iCol = 0; iCol != 4; iCol++) {
				r[iRow][iCol] = m[iCol][iRow];
			}
		return r;
	}
	glm::vec3 convert(aiVector3D& v) {
		return glm::vec3(v.x, v.y, v.z);
	}
	glm::quat convert(aiQuaternion& q) {
		return glm::quat(q.w, q.x, q.y, q.z);
	}
	glm::vec3 convert(aiColor3D& c) {
		return glm::vec3(c.r, c.g, c.b);
	}
	void loadMaterial(const aiMaterial* pmat, Material& m) {
		aiColor3D diff;
		pmat->Get(AI_MATKEY_COLOR_DIFFUSE, diff) ;
		m.diffuse = convert(diff);

		aiColor3D spec;
		pmat->Get(AI_MATKEY_COLOR_SPECULAR, spec) ;
		m.specular = convert(spec);

		aiColor3D ambi;
		pmat->Get(AI_MATKEY_COLOR_AMBIENT, ambi);
		m.ambient = convert(ambi);

		aiColor3D emissie;
		pmat->Get(AI_MATKEY_COLOR_EMISSIVE, emissie);
		m.emissive = convert(emissie);

		aiColor3D transparent;
		pmat->Get(AI_MATKEY_COLOR_TRANSPARENT, transparent);
		m.transparent = convert(transparent);

		pmat->Get(AI_MATKEY_SHININESS, m.shininess);
		
		int two_sided = 0;
		pmat->Get(AI_MATKEY_TWOSIDED, two_sided);
		m.twosided = two_sided != 0;

		uint32_t num_diffuse_texture = pmat->GetTextureCount(aiTextureType_DIFFUSE);
		if(num_diffuse_texture) {
			aiString path;
			if (pmat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				m.tex_diff = path.C_Str();
			}
		}
	}

	void loadVertexInfo(const aiMesh* pmesh, Mesh& m, uint32_t mesh_idx) {
		assert(pmesh->HasFaces());
		assert(pmesh->HasNormals());
		assert(pmesh->HasPositions());

		m.vertex.resize(pmesh->mNumVertices);
		m.normal.resize(pmesh->mNumVertices);

		auto iter = pre_translate_index_.find(mesh_idx);
		if (iter != pre_translate_index_.end()) {
			m.pre_trans =  convert(pre_translate_matrix_[iter->second]);
		}

		for (uint32_t i = 0; i < pmesh->mNumVertices; ++i) {
			aiVector3D v = pmesh->mVertices[i];
			m.vertex[i] = glm::vec3(m.pre_trans * glm::vec4(v.x, v.y, v.z, 1.0f));

			aiVector3D n = pmesh->mNormals[i];
			m.normal[i] = glm::normalize(glm::vec3(m.pre_trans * glm::vec4(n.x, n.y, n.z, 0.0f)));

		}

		// only support first channly
		m.texCoord.clear();
		if (pmesh->HasTextureCoords(0)) {
			m.texCoord.resize(pmesh->mNumVertices);
			for (uint32_t v = 0; v < pmesh->mNumVertices; ++v) {
				m.texCoord[v] = glm::vec2(convert(pmesh->mTextureCoords[0][v]));
			}
		}
		
	

		m.color.resize(pmesh->mNumVertices, glm::vec4(1.0f));
		if (pmesh->HasVertexColors(0)) {
			for (uint32_t i = 0; i < pmesh->mNumVertices; ++i) {
				aiColor4D c = pmesh->mColors[0][i];
				m.color[i] = glm::vec4(c.r, c.g, c.b, c.a);
			}
		}

		
	}

	void loadIndexInfo(const aiMesh* pmesh, Mesh& m) {

		assert(pmesh->HasFaces());
		assert(pmesh->mFaces[0].mNumIndices == 3);

		m.index.resize(pmesh->mNumFaces * 3);
		for (uint32_t i = 0; i < pmesh->mNumFaces; i++) {
			m.index[i * 3] = pmesh->mFaces[i].mIndices[0];
			m.index[i * 3 + 1] = pmesh->mFaces[i].mIndices[1];
			m.index[i * 3 + 2] = pmesh->mFaces[i].mIndices[2];
		}

	}

	void addJointID(Mesh&m, int32_t boneID, float weight, int verID) {
		for (int i = 0; i < 4; ++i) {
			if (m.joint_weight[verID][i] == 0.0f) {
				m.joint_id[verID][i] = boneID;
				m.joint_weight[verID][i] = weight;
				return;
			}
		}
		assert(0);
	}

	void loadJoint(const aiMesh* pmesh, Mesh& m, Skeleton& skeleton) {
		if (!pmesh->HasBones()) return;

		m.joint_id.resize(m.vertex.size(), glm::ivec4(0, 0, 0, 0));
		m.joint_weight.resize(m.vertex.size(), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		for (uint32_t i = 0; i < pmesh->mNumBones; ++i) {
			aiBone* b = pmesh->mBones[i];
			std::string bone_name = b->mName.C_Str();

			std::pair<Skeleton::Joint*, int> r = skeleton.findJointByName(bone_name);
			Skeleton::Joint* j = r.first;

			assert(j != nullptr&&b->mNumWeights != 0);

			Skeleton::EffectiveJoint ej;
			ej.offset = convert(b->mOffsetMatrix);
			int e_idx = skeleton.addEffectiveJoint(r.second, ej);

			for (uint32_t iWeight = 0; iWeight < b->mNumWeights; ++iWeight) {
				addJointID(m, e_idx,
						   b->mWeights[iWeight].mWeight,
						   b->mWeights[iWeight].mVertexId);
			}

		}
	}

	void loadAnimation(const aiAnimation* anim, Skeleton& skeleton) {
		Skeleton::AnimationClip channel;
		channel.duration = static_cast<float>(anim->mDuration);
		channel.ticks_per_second =
			anim->mTicksPerSecond == 0.0f ?
			0.25f : static_cast<float>(anim->mTicksPerSecond);

		channel.samples.resize(anim->mNumChannels);

		for (uint32_t i = 0; i < anim->mNumChannels; ++i) {
			Skeleton::JointPose* pose = &channel.samples[i];
			aiNodeAnim* node_anim = anim->mChannels[i];

			pose->translate_key.resize(node_anim->mNumPositionKeys);

			for (uint32_t keyT = 0; keyT != node_anim->mNumPositionKeys; keyT++) {
				pose->translate_key[keyT].translate =
					convert(node_anim->mPositionKeys[keyT].mValue);
				pose->translate_key[keyT].time =
					static_cast<float>(node_anim->mPositionKeys[keyT].mTime);
			}

			pose->rotate_key.resize(node_anim->mNumRotationKeys);

			for (uint32_t keyR = 0; keyR != node_anim->mNumRotationKeys; keyR++) {
				pose->rotate_key[keyR].rotate =
					convert(node_anim->mRotationKeys[keyR].mValue);
				pose->rotate_key[keyR].time =
					static_cast<float>(node_anim->mRotationKeys[keyR].mTime);
			}

			std::pair<Skeleton::Joint*, int> r = skeleton.findJointByName(node_anim->mNodeName.C_Str());
			Skeleton::Joint* j = r.first;
			j->pose_id[j->num_of_pose++] = i;
			assert(j->num_of_pose == 1);
		}

		skeleton.addAnimation(std::move(channel));
	}

	void loadPreTransform(aiNode* root) {
		pre_translate_matrix_.clear();
		pre_translate_index_.clear();
		pre_translate_matrix_.insert(std::make_pair(root->mName.C_Str(), root->mTransformation));
		updateTranslateIndex(root);
		_loadPreTransform(root);
	}

	void updateTranslateIndex(aiNode* root) {
		for (uint32_t i = 0; i != root->mNumMeshes; ++i) {
			pre_translate_index_.insert(std::make_pair(root->mMeshes[i], root->mName.C_Str()));
		}
	}

	void _loadPreTransform(aiNode* root) {
		assert(pre_translate_matrix_.find(root->mName.C_Str()) != pre_translate_matrix_.end());
		for (size_t i = 0; i < root->mNumChildren; i++) {
			pre_translate_matrix_.insert(std::make_pair(root->mChildren[i]->mName.C_Str(),
								  pre_translate_matrix_[root->mName.C_Str()]
								  * root->mChildren[i]->mTransformation));
			updateTranslateIndex(root->mChildren[i]);
		}
		for (size_t i = 0; i < root->mNumChildren; ++i) {
			_loadPreTransform(root->mChildren[i]);
		}
	}

	Assimp::Importer importer_;
	std::unordered_map<std::string, aiMatrix4x4> pre_translate_matrix_;
	std::unordered_map<uint32_t, std::string> pre_translate_index_;
};

AssimpCodec::AssimpCodec()
	:p_Imp_(nullptr) {
	p_Imp_ = new AssimpCodecImp();
}

AssimpCodec::~AssimpCodec() {
	if (p_Imp_) delete p_Imp_;
}

void AssimpCodec::decode(const std::string& filename) {
	p_Imp_->decode(filename);
}

void AssimpCodec::load(std::vector<Mesh>& mesh,
					   std::vector<Material>& material,
					   Skeleton& skeleton,
					   bool& has_skeleton) {
	p_Imp_->load(mesh, material, skeleton, has_skeleton);
}

SWORD_END
