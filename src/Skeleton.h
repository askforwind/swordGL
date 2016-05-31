/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: Skeleton.h
#
# Last modified:2016-4-24
#
# Description:
#
#=====================================*/

#ifndef SWORD_SKELETON_H__
#define SWORD_SKELETON_H__

#include "Platform.h"
#include "glm/gtx/quaternion.hpp"
#include <vector>
#include <unordered_map>
#include <string>

struct aiNode;
SWORD_BEGIN


class SWORD_EXPORT Skeleton {
public:
	// to much data,high cache miss!!!
	struct Joint {
		uint16_t children_begin;
		uint16_t children_num;
		glm::mat4 transform;
		std::string name;
		int effective_joint_id;
		int pose_id[5];
		int num_of_pose;
	};

	struct EffectiveJoint {
		glm::mat4 offset;
	};

	struct RotateKey {
		glm::quat rotate;
		float time;
	};

	struct TranslateKey {
		glm::vec3 translate;
		float time;
	};

	struct JointPose {
		std::vector<RotateKey> rotate_key;
		std::vector<TranslateKey> translate_key;
		//  TODO: Listen
	};

	struct AnimationClip {
		float duration;
		float ticks_per_second;
		std::vector<JointPose> samples;
	};

	void buildJointTree(const aiNode* root);
	
	std::pair<Joint*,int> findJointByName(const std::string& name);

	std::vector<glm::mat4>&  updateAnimation(float time_in_second);
	
	size_t get_joints_nums()const { return joint_tree_.size(); }

	size_t get_effectiv_joints_nums()const { return effective_.size(); }

	void addAnimation(AnimationClip& anim) { animations_.push_back(anim); }
	
	void addAnimation(AnimationClip&& anim);

	const std::string& self_id()const { return self_id_; };

	void set_self_id(const std::string& id) { self_id_ = id; }

	int addEffectiveJoint(int joint_idx, EffectiveJoint& j);

	std::vector<glm::mat4>& getAnimationMatrix() { return animation_matrix_; }

	// this function is overload for copy and move
	void set_joint_tree(const std::vector<Joint>& jt);
	// this function is overload for copy and move
	void set_joint_map(const std::unordered_map<std::string, int>& jm);
	// this function is overload for copy and move
	void set_animation_clips(const std::vector<AnimationClip>& ac);
	// this function is overload for copy and move
	void set_effective_joint(const std::vector<EffectiveJoint>& ej);

	// this function is overload for copy and move
	void set_joint_tree(std::vector<Joint>&& jt);
	// this function is overload for copy and move
	void set_joint_map(std::unordered_map<std::string, int>&& jm);
	// this function is overload for copy and move
	void set_animation_clips(std::vector<AnimationClip>&& ac);
	// this function is overload for copy and move
	void set_effective_joint(std::vector<EffectiveJoint>&& ej);

	void copyTo(Skeleton& rhs);

	void moveTo(Skeleton& rhs);

private:
	void calcInterpolatedRotation(float anim_time, const JointPose& pose, glm::quat& q);

	void calcInterpolatedTranslation(float anim_time, const JointPose& pose, glm::vec3& v);

	std::vector<glm::mat4> animation_matrix_;

	void _updateAnimation(float animation_time, int joint_idx,
						  int anim_idx,glm::mat4 parent_transform);

	void _buildJointTree(const aiNode* node, int index);

	int getJointRotateKey(float anim_time, const JointPose&)const;

	int getJointTransformKey(float anim_time, const JointPose&)const;

	std::vector<Joint> joint_tree_;

	std::vector<AnimationClip> animations_;

	std::vector<EffectiveJoint> effective_;

	std::unordered_map<std::string, int> joint_map_;

	std::string self_id_;
};

SWORD_END

#endif // SWORD_SKELETON_H__