/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: Material.h
#
# Last modified:2016-4-15
#
# Description:
#
#=====================================*/

#ifndef SWORD_MATERIAL_H__
#define SWORD_MATERIAL_H__
#include "Platform.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <string>

SWORD_BEGIN

class SWORD_EXPORT Material {
public:
	Material() ;
	~Material() = default;

	struct Info {
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 ambient;
		glm::vec3 transparent;
		bool wireframe;
		bool twosided;
		std::string texture_id[4];
	};

	void set_diffuse(const glm::vec3& diff);

	void set_specular(const glm::vec3& spec);

	void set_ambient(const glm::vec3& ambient);

	void set_transparent(const glm::vec3& transparent);

	void set_wireframe(bool used);

	void set_twosided(bool b);

	void set_texture(std::string& id, uint8_t idx);

	const Info* get_info()const { return &info_; }

protected:
	Info info_;
};

SWORD_END

#endif // SWORD_MATERIAL_H__