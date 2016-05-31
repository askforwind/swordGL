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

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include <vector>

namespace SWORD {

	struct Mesh {
		std::vector<glm::vec3> vertex;
		std::vector<glm::vec3> normal;
		std::vector<glm::vec4> color;
		std::vector<uint32_t> index;
		std::vector<glm::vec2> texCoord;
		std::vector<glm::ivec4> joint_id;
		std::vector<glm::vec4> joint_weight;
		glm::mat4 pre_trans;
		std::string material_id;
		std::string skeleton_id;
		std::string id;
		//glm::mat4 pre_transform;
		std::string self_id() { return id; };
		void set_self_id(const std::string& id) { this->id = id; }
	};



	struct Material {
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 ambient;
		glm::vec3 emissive;
		glm::vec3 transparent;
		
		float shininess;
		bool twosided;
		std::string tex_diff;
		std::string id;
		std::string self_id() { return id; };
		void set_self_id(const std::string& id) { this->id = id; }
	};



	struct Module {
		std::vector<std::string> sub_meshs;
		std::string id;
		std::string self_id() { return id; };
		void set_self_id(const std::string& id) { this->id = id; }
	};


}

#endif // SWORD_RESOURCEDEF_H__