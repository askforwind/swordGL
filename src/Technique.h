
/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: Technique.h
#
# Last modified:2016-4-28
#
# Description:
#
#=====================================*/

#ifndef SWORD_TECHNIQUE_H__
#define SWORD_TECHNIQUE_H__

#include "Platform.h"
#include "glm/glm.hpp"
#include "CommandDef.h"
#include <string>

SWORD_BEGIN

class TechniqueManager;

class SWORD_EXPORT Technique {
public:

	enum ShaderType {
		VS = 1,
		PS = 2
	};

	struct AttributeInfo {
		Attribute attr;
		int32_t location;
	};

	struct UniformInfo {
		UniformTarget target;
		int32_t location;
	};

	Technique();

	~Technique();

	bool init();

	void enable();

	void disable();

	bool addShader(ShaderType type, const std::string& filename);

	void addAttributeLayout(Attribute attr, int locaton);

	void addUniformLayout(UniformTarget target,  int location);

	const AttributeInfo* get_attribute_layout()const;

	const UniformInfo* get_uniform_layout()const;

	size_t get_attribute_layout_size()const;

	size_t get_uniform_layout_size()const;

	int32_t get_prog()const { return shader_prog_; }

	bool finalize();

	void deinit();

	int32_t get_attrib_location(const std::string& attrib_name)const;

	int32_t get_uniform_location(const std::string& uniform_name)const;

	void set_uniform_mat(int32_t location, const glm::mat4& m)const;

	void set_uniform_mat(int32_t location, const float* m)const;

	void set_uniform_vec3(int32_t location, const glm::vec3& v)const;

	void set_uniform_vec3(int32_t location, const float* v)const;

	void set_uniform_int(int32_t location, const int value)const;

	void set_uniform_float(int32_t location, const float value)const;

	void set_multi_uniform_mat(int32_t location, size_t size, const glm::mat4& first)const;

	void set_multi_uniform_mat(int32_t location, size_t size, const float* m)const;

	const std::string& self_id()const { return id; }

	friend class TechniqueManager;

private:
	AttributeInfo attr_layout_[kSHADER_ATTRIBUTE_NUM];

	size_t attr_layout_size_;

	UniformInfo uniform_layout_[kSHADER_UNIFORM_NUM];

	size_t uniform_layout_size_;

	void set_self_id(const std::string& id) { this->id = id; }


	std::string id;

	void setShaderObj(ShaderType type, int32_t obj);

	int32_t shader_prog_;

	int32_t vs_obj_;
	int32_t ps_obj_;
};

SWORD_END

#endif // SWORD_TECHNIQUE_H__