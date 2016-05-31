
#include <fstream>
#include <assert.h>
#include "Technique.h"
#include "GL\glew.h"
#include "config.h"
#include "LogManager.h"
#include "glm\gtc\type_ptr.hpp"

using std::string;
using std::ifstream;

SWORD_BEGIN

Technique::Technique()
	:shader_prog_(0)
	, vs_obj_(0)
	, ps_obj_(0)
	,attr_layout_size_(0)
	,uniform_layout_size_(0){
}

Technique::~Technique() {
	deinit();
}

void Technique::deinit() {
	if (shader_prog_) CHECK_GL_ERROR(glDeleteProgram(shader_prog_));
	shader_prog_ = 0;
	setShaderObj(VS, 0);
	setShaderObj(PS, 0);
}

void Technique::setShaderObj(ShaderType type, int32_t obj) {
	
	int32_t * shader_obj = &shader_prog_ + type;

	assert(shader_obj == &vs_obj_ || shader_obj == &ps_obj_);

	if (*shader_obj)
		CHECK_GL_ERROR(glDeleteShader(*shader_obj));
		
	*shader_obj = 0;
}

bool Technique::init() {
	deinit();

	CHECK_GL_ERROR(shader_prog_ = glCreateProgram());

	assert(shader_prog_);

	if (shader_prog_ == 0) {
		WIND_LOG_ERROR(DEFAULT_WIND_LOGGER,
					   "can't create shader program");
		return false;
	}

	for (int i = 0; i < kSHADER_ATTRIBUTE_NUM; i++) {
		attr_layout_[i].attr = Attribute::UNASSIGNED;
		attr_layout_[i].location = kINVALID_SHADER_LOCATION;
	}
	attr_layout_size_ = 0;

	for (int i=0;i<kSHADER_UNIFORM_NUM;i++)
	{
		uniform_layout_[i].target = UniformTarget::UNKNOWN_TARGET;
		uniform_layout_[i].location = kINVALID_SHADER_LOCATION;
	}
	uniform_layout_size_ = 0;

	setShaderObj(PS, 0);
	setShaderObj(VS, 0);
	return true;
}

namespace {

	bool readfile(const std::string& filename, string& out) {
		ifstream input(filename);

		if (!input) return false;

		out.clear();
		out.append(std::istreambuf_iterator<char>(input),
				   std::istreambuf_iterator<char>());
		return true;
	}

	GLenum convert(Technique::ShaderType type) {
		GLenum ret = 0;

		switch (type) {
		case SWORD::Technique::VS:ret = GL_VERTEX_SHADER;
			break;
		case SWORD::Technique::PS: ret = GL_FRAGMENT_SHADER;
			break;
		default: assert(0 && "Unknown Shader Type");
			break;
		}

		return ret;

	}

}

// Use this method to add shaders to the program. When finished - call finalize()
bool Technique::addShader(ShaderType type, const std::string& filename) {
	string s;

	if (!readfile(filename, s))	  return false;

	GLuint shader_obj = 0;
	CHECK_GL_ERROR(shader_obj = glCreateShader(convert(type)));

	assert(shader_obj&&"create shader failure");

	if (!shader_obj) {
		WIND_LOG_ERROR(DEFAULT_WIND_LOGGER,
					   "can't create shader:" +
					   filename);
		return false;
	}

	const char* data = s.c_str();
	const GLchar** p = &data;
	const GLint length = static_cast<GLint>(s.size());
	CHECK_GL_ERROR(glShaderSource(shader_obj, 1, p, &length));
	CHECK_GL_ERROR(glCompileShader(shader_obj));

	GLint success;
	CHECK_GL_ERROR(glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &success));

	if (!success) {
		GLchar info[1024];
		CHECK_GL_ERROR(glGetShaderInfoLog(shader_obj, 1024, NULL, info));
		fprintf(stderr, "Error compiling '%s': '%s'\n", filename.c_str(), info);
		assert(0);
		return false;
	}

	CHECK_GL_ERROR(glAttachShader(shader_prog_, shader_obj));
	setShaderObj(type, shader_obj);

	return true;
}

void Technique::addAttributeLayout(Attribute type, int location) {
	assert(attr_layout_size_ <= kSHADER_ATTRIBUTE_NUM);
	attr_layout_[attr_layout_size_].attr = type;
	attr_layout_[attr_layout_size_].location = location;
	attr_layout_size_++;
}

void Technique::addUniformLayout(UniformTarget target, int location) {
	assert(uniform_layout_size_ <= kSHADER_UNIFORM_NUM);
	uniform_layout_[uniform_layout_size_].target = target;
	uniform_layout_[uniform_layout_size_].location = location;
	uniform_layout_size_++;
}

const Technique::AttributeInfo * Technique::get_attribute_layout() const {
	return attr_layout_;
}

const Technique::UniformInfo * Technique::get_uniform_layout() const {
	return uniform_layout_;
}

size_t Technique::get_attribute_layout_size() const {
	return attr_layout_size_;
}

size_t Technique::get_uniform_layout_size() const {
	return uniform_layout_size_;
}


// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Technique::finalize() {
	GLint success = 0;
	GLchar info[1024] = { 0 };

	CHECK_GL_ERROR(glLinkProgram(shader_prog_));

	CHECK_GL_ERROR(glGetProgramiv(shader_prog_, GL_LINK_STATUS, &success));
	if (!success) {
		CHECK_GL_ERROR(glGetProgramInfoLog(shader_prog_, sizeof(info), NULL, info));
		fprintf(stderr, "Error linking shader program: '%s'\n", info);
		return false;
	}

	CHECK_GL_ERROR(glValidateProgram(shader_prog_));
	CHECK_GL_ERROR(glGetProgramiv(shader_prog_, GL_VALIDATE_STATUS, &success));
	if (!success) {
		CHECK_GL_ERROR(glGetProgramInfoLog(shader_prog_, sizeof(info), NULL, info));
		// that's useful in cmd
		fprintf(stderr, "Invalid shader program: '%s'\n", info);
		WIND_LOG_ERROR(DEFAULT_WIND_LOGGER, std::string("Invalid shader program:") + info);
		return false;
	}

	setShaderObj(VS, 0);
	setShaderObj(PS, 0);

	return glGetError() == GL_NO_ERROR;
}

void Technique::enable() {
	assert(shader_prog_);
	assert(!vs_obj_);
	assert(!ps_obj_);
	CHECK_GL_ERROR(glUseProgram(shader_prog_));
}

void Technique::disable() {
	CHECK_GL_ERROR(glUseProgram(0));
}

int32_t Technique::get_attrib_location(const std::string& attrib_name)const {
	assert(shader_prog_);

	int32_t location = 0;
	CHECK_GL_ERROR(location = glGetAttribLocation(shader_prog_, attrib_name.c_str()));
	if (location == kINVALID_SHADER_LOCATION) {
		WIND_LOG_WARN(DEFAULT_WIND_LOGGER,
					  std::string("Unable to get the location of uniform:") + attrib_name.c_str());
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", attrib_name.c_str());
	}
	return location;
}

int32_t Technique::get_uniform_location(const std::string& uniform_name) const {
	assert(shader_prog_);

	int32_t location = 0;
	CHECK_GL_ERROR(location = glGetUniformLocation(shader_prog_, uniform_name.c_str()));
	if (location == kINVALID_SHADER_LOCATION) {
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", uniform_name.c_str());
	}
	return location;
}

void Technique::set_uniform_mat(int32_t location, const glm::mat4 & m) const{
	CHECK_GL_ERROR(glUniformMatrix4fv(location, 1, false, glm::value_ptr(m)));
}

void Technique::set_uniform_mat(int32_t location, const float * m) const {
	CHECK_GL_ERROR(glUniformMatrix4fv(location, 1, false, m));
}

void Technique::set_uniform_vec3(int32_t location, const glm::vec3 & v)const {
	CHECK_GL_ERROR(glUniform3fv(location, 1, glm::value_ptr(v)));
}

void Technique::set_uniform_vec3(int32_t location, const float * v) const {
	CHECK_GL_ERROR(glUniform3fv(location, 1, v));
}

void Technique::set_uniform_int(int32_t location, const int value)const {
	CHECK_GL_ERROR(glUniform1i(location, value));
}

void Technique::set_uniform_float(int32_t location, const float value)const {
	CHECK_GL_ERROR(glUniform1f(location, value));
}

void Technique::set_multi_uniform_mat(int32_t location, size_t size, const glm::mat4& first)const {
	CHECK_GL_ERROR(glUniformMatrix4fv(location, size, false, glm::value_ptr(first)));
}

void Technique::set_multi_uniform_mat(int32_t location, size_t size, const float * m) const {
	CHECK_GL_ERROR(glUniformMatrix4fv(location, size, false, m));
}


SWORD_END