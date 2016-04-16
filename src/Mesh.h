/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: Mesh.h
#
# Last modified:2016-4-15
#
# Description:
#
#=====================================*/

#ifndef SWORD_MESH_H__
#define SWORD_MESH_H__

#include "Platform.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <vector>

SWORD_BEGIN

class SWORD_EXPORT Mesh {
public:
	Mesh();
	~Mesh() = default;

	struct Info {
		std::vector<glm::vec3> vertex;
		std::vector<uint16_t> index;
		std::vector<glm::vec4> color;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normal;
		std::string material_id;
	};
	void set_material(const std::string& id);

	void readVertex(const std::vector<glm::vec3>& vertex);

	void readVertex(std::vector<glm::vec3>&& vertex);

	void readIndex(const std::vector<uint16_t>& index);

	void readIndex(std::vector<uint16_t>&& index);

	void readColor(const std::vector<glm::vec4>& color);

	void readColor(std::vector<glm::vec4>&& color);

	void readUV(const std::vector<glm::vec2>& uv);

	void readUV(std::vector<glm::vec2>&& uv);

	void readNormal(const std::vector<glm::vec3>& normal);

	void readNormal(std::vector<glm::vec3>&& normal);

	const Info* get_info()const { return &info_; };
protected:

	Info info_;
};

SWORD_END

#endif // SWORD_MESH_H__