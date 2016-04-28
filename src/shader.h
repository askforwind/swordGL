
/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: shader.hpp
#
# Last modified:2016-4-28
#
# Description:
#
#=====================================*/

#ifndef SWORD_SHADER_H__
#define SWORD_SHADER_H__

#include "GL/glew.h"
#include <list>
class Technique {
public:

	Technique();

	virtual ~Technique();

	virtual bool Init();

	void Enable();

	bool AddShader(GLenum ShaderType, const char* pFilename);

	bool Finalize();

	GLint GetAttribLocation(const char* pAttribName);

	GLint GetUniformLocation(const char* pUniformName);

	GLint GetProgramParam(GLint param);

	GLuint m_shaderProg;

private:

	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shaderObjList;
};

#endif // SWORD_SHADER_H__