#include <map>
#include "AssimpCodec.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

SWORD_BEGIN

class AssimpCodecImp {
public:
	void decode(const std::string& filename) {
		importer_.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);
	}

	void load(std::vector<Mesh>& mesh,
			  std::vector<Material>& material,Skeleton& skeleton) {
		const aiScene* scene = importer_.GetScene();

		skeleton.buildJointTree(scene->mRootNode);

		mesh.resize(scene->mNumMeshes);
		for (uint32_t i = 0; i < scene->mNumMeshes; ++i) {
			const aiMesh* ms = scene->mMeshes[i];
			loadIndexInfo(ms, mesh[i]);
			loadVertexInfo(ms, mesh[i]);
			loadJoint(ms, mesh[i], skeleton);
			mesh[i].material_id = std::to_string(ms->mMaterialIndex);
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
		pmat->Get(AI_MATKEY_COLOR_DIFFUSE, diff);
		m.diffuse = convert(diff);

		aiColor3D spec;
		pmat->Get(AI_MATKEY_COLOR_SPECULAR, spec);
		m.specular = convert(spec);

		aiColor3D ambi;
		pmat->Get(AI_MATKEY_COLOR_AMBIENT, ambi);
		m.ambient = convert(ambi);

		if (pmat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path;
			if (pmat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				m.tex_diff = path.C_Str();
			}
		}
	}

	void loadVertexInfo(const aiMesh* pmesh, Mesh& m) {
		assert(pmesh->HasFaces());
		assert(pmesh->HasNormals());
		assert(pmesh->HasTextureCoords(0));
		assert(pmesh->HasPositions());

		m.vertex.resize(pmesh->mNumVertices);
		m.normal.resize(pmesh->mNumVertices);
		m.texCoord.resize(pmesh->mNumVertices);

		for (uint32_t i = 0; i < pmesh->mNumVertices; ++i) {
			aiVector3D v = pmesh->mVertices[i];
			m.vertex[i] = glm::vec3(v.x, v.y, v.z);

			aiVector3D n = pmesh->mNormals[i];
			m.normal[i] = glm::vec3(n.x, n.y, n.z);

			aiVector3D t = pmesh->mTextureCoords[0][i];
			m.texCoord[i] = glm::vec2(t.x, t.y);
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

	void addJointID(Mesh&m, int32_t boneID, float weight,int verID) {
		for (int i = 0; i < 4; ++i) {
			if (m.joint_weight[verID][i] == 0.0f) {
				m.joint_id[verID][i] = boneID;
				m.joint_weight[verID][i] = weight;
				return;
			}
		}
		assert(0);
	}

	void loadJoint(const aiMesh* pmesh, Mesh& m,Skeleton& skeleton) {
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

			for (uint32_t iWeight=0;iWeight<b->mNumWeights;++iWeight)
			{
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

	Assimp::Importer importer_;
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
					   Skeleton& skeleton) {
	p_Imp_->load(mesh, material, skeleton);
}

SWORD_END
