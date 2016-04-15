/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: Node.h
#
# Last modified:2016-4-13
#
# Description:
#
#=====================================*/

#ifndef SWORD_NODE_H_
#define SWORD_NODE_H_
#include"Platform.h"
#include<glm\gtx\transform.hpp>

SWORD_BEGIN

class SWORD_EXPORT Node
{
public:
	
	enum Coord {
		Local,
		World,
		Parent
	};
	
	Node();

	virtual ~Node() {};

	glm::vec3 get_position()const;

	glm::mat4 get_model_matrix()const;

	virtual void set_position(const glm::vec3& pos,const Coord& coord=Coord::World);

	virtual void translate(const glm::vec3& trans_vec,const Coord& coord);

	virtual void rotate(const float& radians, const glm::vec3 & axis,const Coord& coord);

	// rotate about z
	virtual void yaw(const float& radians,const Coord& coord);
	// rotate about y
	virtual void pitch(const float& radians,const Coord& coord);
	// rotate about x
	virtual void roll(const float& radians,const Coord& coord);

protected:
	glm::mat4 world_;
	glm::mat4 local_;
	//TODO : parent
};

SWORD_END

#endif // SWORD_NODE_H_
