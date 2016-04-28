#include "Skeleton.h"
#include "assimp/scene.h"
#include "glm/gtx/transform.hpp"
#include <algorithm>
#include <iostream>
SWORD_BEGIN

namespace {
	void convertMatrix(const aiMatrix4x4& am, glm::mat4& m) {
		for (int iRow = 0; iRow < 4; ++iRow)
			for (int iCol = 0; iCol < 4; ++iCol) {
				m[iRow][iCol] = am[iCol][iRow];
			}
	}
	void convert(const aiNode* node, Skeleton::Joint& joint) {
		joint.children_begin = 0;
		joint.children_num = node->mNumChildren;
		joint.num_of_pose = 0;
		joint.pose_id[0] = -1;
		joint.name = std::string(node->mName.C_Str());
		joint.effective_joint_id = -1;
		convertMatrix(node->mTransformation, joint.transform);
	}
}

Skeleton & Skeleton::operator=(const Skeleton & rhs) {
	if (this == &rhs) return *this;

	this->animations_ = rhs.getAnimationsClip();
	this->joint_tree_ = rhs.getJointTree();
	this->joint_map_ = rhs.getJointMap();
	this->animation_matrix_ = rhs.getAnimationMatrix();
	this->self_id_ = rhs.self_id();
	this->effective_ = rhs.getEffectiveJoint();
	return *this;
}

void Skeleton::buildJointTree(const aiNode * root_node) {
	Joint root_joint;
	convert(root_node, root_joint);
	
	joint_tree_.clear();
	joint_tree_.push_back(root_joint);
	
	joint_map_.clear();
	joint_map_.insert(std::make_pair(root_joint.name, 0));

	_buildJointTree(root_node, 0);

	animation_matrix_.resize(joint_tree_.size());
}

std::pair<Skeleton::Joint*, int>  Skeleton::findJointByName(const std::string & name) {
	auto iter = joint_map_.find(name);
	return iter == joint_map_.end() ?
		std::make_pair(nullptr, -1)
		: std::make_pair(&joint_tree_[iter->second], iter->second);
}

std::vector<glm::mat4>& 
Skeleton::updateAnimation(float time_in_second) {
	assert(!animations_.empty());
	float time_in_ticks = animations_[0].ticks_per_second*time_in_second;
	float animation_time = fmodf(time_in_ticks, animations_[0].duration);

	if (effective_.size() != animation_matrix_.size())
		animation_matrix_.resize(effective_.size());

	_updateAnimation(animation_time, 0, 0,glm::mat4(1.0f));

	return animation_matrix_;
}

void Skeleton::_updateAnimation(float animation_time, int joint_idx ,
								int anim_idx,glm::mat4 parent_transform) {
	Joint* j = &joint_tree_[joint_idx];
	glm::mat4 node_transform = j->transform;

	if (anim_idx<j->num_of_pose)
	{
		const std::vector<JointPose>& sample = animations_[anim_idx].samples;
		int pose_idx = j->pose_id[anim_idx];

		int Rframe = sample[pose_idx].rotate_key.size() == 1 ?
			0 :
			getJointRotateKey(animation_time, sample[pose_idx]);

		int Tframe = sample[pose_idx].translate_key.size() == 1 ?
			0 :
			getJointTransformKey(animation_time, sample[pose_idx]);

		const glm::quat& Q = sample[pose_idx].rotate_key[Rframe].rotate;
		glm::mat4 R = glm::toMat4(Q);
		const glm::vec3& V = sample[pose_idx].translate_key[Tframe].translate;
		glm::mat4 T = glm::translate(glm::mat4(1.0f), V);
		node_transform = T * R;
	}

	glm::mat4 world_transform = parent_transform*node_transform;
	if (j->effective_joint_id >= 0)
		this->animation_matrix_[j->effective_joint_id] =
		world_transform*effective_[j->effective_joint_id].offset;

	for (size_t i = 0; i != j->children_num; ++i) {
		_updateAnimation(animation_time, j->children_begin + i,
						 anim_idx, world_transform);
	}

}

void Skeleton::addAnimation(AnimationClip && anim) {
	animations_.push_back(AnimationClip());
	animations_.back().duration = anim.duration;
	animations_.back().ticks_per_second = anim.ticks_per_second;
	animations_.back().samples = std::move(anim.samples);
}

int Skeleton::addEffectiveJoint(int joint_idx, EffectiveJoint & j) {
	

	assert(joint_idx < joint_tree_.size());

	assert(joint_tree_[joint_idx].effective_joint_id < 0
		   || glm::equal(j.offset[0], effective_[joint_tree_[joint_idx].effective_joint_id].offset[0])
		   == glm::bvec4(true, true, true, true));
	assert(joint_tree_[joint_idx].effective_joint_id < 0
		   || glm::equal(j.offset[1], effective_[joint_tree_[joint_idx].effective_joint_id].offset[1])
		   == glm::bvec4(true, true, true, true));
	assert(joint_tree_[joint_idx].effective_joint_id < 0
		   || glm::equal(j.offset[2], effective_[joint_tree_[joint_idx].effective_joint_id].offset[2])
		   == glm::bvec4(true, true, true, true));
	assert(joint_tree_[joint_idx].effective_joint_id < 0
		   || glm::equal(j.offset[3], effective_[joint_tree_[joint_idx].effective_joint_id].offset[3])
		   == glm::bvec4(true, true, true, true));

	
	int e_id = joint_tree_[joint_idx].effective_joint_id;
	if (e_id >= 0) {
		return e_id;
	}
	
	effective_.push_back(j);
	joint_tree_[joint_idx].effective_joint_id = effective_.size() - 1;

	return effective_.size() - 1;
}

void Skeleton::_buildJointTree(const aiNode* node, int index) {

	assert(index < joint_tree_.size());
	joint_tree_[index].children_begin = joint_tree_.size();

	for (int iChild = 0; iChild != node->mNumChildren; ++iChild) {
		Joint cj;
		convert(node->mChildren[iChild], cj);
		//assert(joint_map_.find(cj.name) == joint_map_.end());
		joint_tree_.push_back(cj);
		joint_map_[cj.name] = joint_tree_.size() - 1;
	}

	for (int i = 0; i != node->mNumChildren; ++i) {
		_buildJointTree(node->mChildren[i], joint_tree_[index].children_begin + i);
	}

}

int Skeleton::getJointRotateKey(float anim_time, const JointPose& pose) const {
	assert(pose.rotate_key.size() > 1);

	auto iter = std::lower_bound(pose.rotate_key.begin() + 1,
								 pose.rotate_key.end(),
								 RotateKey{ glm::quat(0.0f, 0.0f, 0.0f, 0.0f), anim_time },
								 [](const RotateKey& k1, const RotateKey& k2)->bool {return k1.time < k2.time; });

	return iter - pose.rotate_key.begin() - 1;
}

int Skeleton::getJointTransformKey(float anim_time, const JointPose& pose) const {
	assert(pose.translate_key.size() > 1);

	auto iter = std::lower_bound(pose.translate_key.begin() + 1,
								 pose.translate_key.end(),
								 TranslateKey{ glm::vec3(0.0f, 0.0f, 0.0f), anim_time },
								 [](const TranslateKey& k1,const TranslateKey& k2)->bool {return k1.time < k2.time; });

	return iter - pose.translate_key.begin() - 1;
}



SWORD_END