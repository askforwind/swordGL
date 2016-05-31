std::map<std::string, int>boneMap;
std::vector<glm::mat4> boneOffset;
std::vector<glm::mat4> boneTrans;
void AddBoneData(uint16_t meshID,uint BoneID,uint16_t verID, float Weight) {
	for (uint i = 0; i < 4; i++) {
		if (Entity[meshID].weight[verID][i] == 0.0f) {
			Entity[meshID].boneID[verID][i] = BoneID;
			Entity[meshID].weight[verID][i]=Weight;
			return;
		}
	}

	 //should never get here - more bones than we have space for
	assert(0);
}

void loadMesh(const aiScene* scene) {
	Entity.resize(scene->mNumMeshes);
	boneOffset.resize(1000);
	for (int i=0;i<scene->mNumMeshes;++i)
	{
		aiMesh* paiMesh = scene->mMeshes[i];

		for (uint k = 0; k < paiMesh->mNumVertices; k++) {
			const aiVector3D* pPos = &(paiMesh->mVertices[k]);
			Entity[i].pos.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		}

		
		Entity[i].weight.resize(paiMesh->mNumVertices);
		Entity[i].boneID.resize(paiMesh->mNumVertices);
		for (int k = 0; k < paiMesh->mNumBones; ++k) {
			aiBone* b = paiMesh->mBones[k];
			int idx;

			if (boneMap.find(b->mName.C_Str()) == boneMap.end()) {
				idx = boneMap.size();
				boneMap[b->mName.C_Str()] = idx;
				boneOffset[idx] = convert(b->mOffsetMatrix);
			}
			else {
				idx = boneMap[b->mName.C_Str()];
			}

			for (int p=0;p<b->mNumWeights;++p)
			{
				AddBoneData(i, idx, b->mWeights[p].mVertexId, b->mWeights[p].mWeight);
			}

		}

		for (uint k = 0; k < paiMesh->mNumFaces; k++) {
			const aiFace& Face = paiMesh->mFaces[k];
			assert(Face.mNumIndices == 3);
			Entity[i].indices.push_back(Face.mIndices[0]);
			Entity[i].indices.push_back(Face.mIndices[1]);
			Entity[i].indices.push_back(Face.mIndices[2]);
		}
	}
}

const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName) {
	for (uint i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}

uint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim) {
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (uint i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}
uint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim) {
	for (uint i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}

void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim) {
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	uint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	Out = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	/*uint NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();*/
}
void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim) {
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}


	uint PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	Out = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	/*uint NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;*/
}

void UpdateNode(float animTime, aiNode* pNode, glm::mat4 parent,const aiScene* m_pScene) {
	string NodeName(pNode->mName.data);

	const aiAnimation* pAnimation = m_pScene->mAnimations[0];

	glm::mat4 NodeTransformation = convert(pNode->mTransformation);

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (boneMap.find(NodeName) != boneMap.end()) {
		uint BoneIndex = boneMap[NodeName];
		if (BoneIndex == 2) {
			int kkkk = 9;
		}
	}

	if (pNodeAnim) {
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, animTime, pNodeAnim);
		glm::quat q(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z);
		glm::mat4 Q = glm::toMat4(q);

		 //Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, animTime, pNodeAnim);
		glm::mat4 TranslationM(1.0f);
		TranslationM = glm::translate(TranslationM, glm::vec3(Translation.x, Translation.y, Translation.z));
		NodeTransformation = glm::mat4(1.0f);
		// Combine the above transformations
		NodeTransformation = TranslationM * Q;
	}

	glm::mat4 GlobalTransformation = parent * NodeTransformation;

	if (boneMap.find(NodeName) != boneMap.end()) {
		uint BoneIndex = boneMap[NodeName];
		boneTrans[BoneIndex] = GlobalTransformation * boneOffset[BoneIndex];
	}

	for (uint i = 0; i < pNode->mNumChildren; i++) {
		UpdateNode(animTime, pNode->mChildren[i], GlobalTransformation, m_pScene);
	}
}

void updateMatrix(float TimeInSeconds, const aiScene* m_pScene, std::vector<glm::mat4>& tranforms) {

	float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);
	boneTrans.resize(boneOffset.size());

	UpdateNode(AnimationTime, m_pScene->mRootNode, glm::mat4(1.0f),m_pScene);

	tranforms.resize(boneOffset.size());

	for (uint i = 0; i < tranforms.size(); i++) {
		tranforms[i] = boneTrans[i];
	}
}


void check(const std::vector<SWORD::Skeleton::Joint>& jt, int idx, aiNode* root) {
	const SWORD::Skeleton::Joint& j = jt[idx];
	assert(root->mName.C_Str() == j.name);
	assert(root->mNumChildren == j.children_num);
	assert(convert(root->mTransformation) == j.transform);

	for (int i=0;i!=j.children_num;++i)
	{
		assert(root->mChildren[i]->mName.C_Str() == jt[i + j.children_begin].name);
		assert(root->mChildren[i]->mNumChildren == jt[i + j.children_begin].children_num);
	}
	for (int i=0;i!=j.children_num;i++)
	{
		check(jt, j.children_begin + i, root->mChildren[i]);
	}
}


void checkTrans(SubMesh& submesh, SWORD::MeshPtr mp,
				const std::vector<SWORD::Skeleton::Joint>& jt,
				const std::vector<glm::mat4>& transform,
				const std::vector<glm::mat4>& mt) {
	for (int i=0;i<submesh.boneID.size();++i)
	{
		assert(glm::equal(submesh.weight[i], mp->joint_weight[i])
			   == glm::bvec4(true, true, true, true));
		for (int k=0;k<4;++k)
		{
			if (submesh.weight[i][k] != 0.0f) {
				
				assert(glm::equal(transform[submesh.boneID[i][k]][0],
					   mt[mp->joint_id[i][k]][0])
					   == glm::bvec4(true, true, true, true));


				assert(glm::equal(transform[submesh.boneID[i][k]][1],
					   mt[mp->joint_id[i][k]][1])
					   == glm::bvec4(true, true, true, true));


				assert(glm::equal(transform[submesh.boneID[i][k]][2],
					   mt[mp->joint_id[i][k]][2])
					   == glm::bvec4(true, true, true, true));


				assert(glm::equal(transform[submesh.boneID[i][k]][3],
					   mt[mp->joint_id[i][k]][3])
					   == glm::bvec4(true, true, true, true));
			}
		}
	}
}