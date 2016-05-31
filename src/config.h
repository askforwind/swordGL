/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-04-06 12:53
#
# Filename: config.h
#
# Description:
#
==================================================================*/
#ifndef SWORD_CONFIG_H_
#define SWORD_CONFIG_H_
#include"Platform.h"

#   if CHECK_OPENGL_FUNCTION
#      define CHECK_GL_ERROR(glFunc) \
{ \
    glFunc; \
    int e = glGetError(); \
    if (e != 0) \
    { \
        WIND_LOG_ERROR(DEFAULT_WIND_LOGGER,gluErrorString(e));\
    } \
}
#   else
#      define CHECK_GL_ERROR(glFunc) glFunc
#   endif

#endif

#define kINVALID_VAO_ID  0
#define kINVALID_VBO_ID  0
#define kINVALID_MAGIC_NUM  0xffffffff
#define kINVALID_SHADER_LOCATION  0xffffffff





