/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: ResourceDef.h
#
# Last modified:2016-4-24
#
# Description:
#
#=====================================*/

#ifndef SWORD_RESOURCEDEF_H__
#define SWORD_RESOURCEDEF_H__

#include "Platform.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <vector>

SWORD_BEGIN

struct Mesh {
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec4> color;
	std::vector<uint32_t> index;
	std::vector<glm::vec2> texCoord;
	std::vector<glm::ivec4> joint_id;
	std::vector<glm::vec4> joint_weight;
	std::string material_id;
	std::string skeleton_id;
	std::string self_id;
};

struct Material {
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;
	glm::vec3 transparent;
	bool wireframe;
	bool twosided;
	std::string tex_opacity[4];
	std::string tex_diff;
	std::string self_id;
};

struct Module {
	std::vector<std::string> sub_meshs;
	std::string self_id;
};


SWORD_END

#endif // SWORD_RESOURCEDEF_H__