/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: Entity.h
#
# Last modified:2016-5-7
#
# Description:
#
#=====================================*/

#ifndef SWORD_ENTITY_H__
#define SWORD_ENTITY_H__

#include "Platform.h"
#include "Timer.h"
#include <glm/glm.hpp>
SWORD_BEGIN

enum Coord {
	WORLD = 0,
	LOCAL = 1
};


class SWORD_EXPORT Entity {
public:
	Entity();

	glm::mat4& get_module_matrix() { return final_; }

	float get_animation_time() { timer_.tick(); return timer_.sinceBeginInSecond(); }

	void set_pos(glm::vec3& m,Coord c);

	glm::vec3 get_pos();

	void translate(glm::vec3& v, Coord c);

	void rotate(glm::quat& q, Coord c);

	void updateModel();
private:
	glm::mat4 model_[2];
	glm::mat4 final_;
	Timer timer_;
};

SWORD_END

#endif // SWORD_ENTITY_H__