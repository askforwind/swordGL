/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-03-22 09:54
#
# Filename: Platform.h
#
# Description: 
#
==================================================================*/
#ifndef SWORD_DEPENDENCE_PLATFORM_H_
#define SWORD_DEPENDENCE_PLATFORM_H_
#include "include/precompile.h"

#if defined(__WIN32__)|| defined (_WIN32)
#   define SWORD_PLATFORM_WIN
#else
#   define SWORD_PLATFORM_LINUX
#endif



#if defined SWORD_PLATFORM_WIN

#	if !SWORD_RELEASE
#		define SWORD_EXPORT
#   elif defined SWORD_BUILD_SHARE
#       define SWORD_EXPORT  __declspec(dllexport)
#   elif defined SWORD_BUILD_STATIC
#       define SWORD_EXPORT
#	
#   endif

#else
#   define SWORD_EXPORT
#endif

#if defined(_DEBUG)||defined(DEBUG)
#   define SWORD_DEBUG_MODE 1
#else
#   define SWORD_DEBUG_MODE 0
#endif

#define SWORD_BEGIN namespace SWORD{
#define SWORD_END }

#endif //SWORD_DEPENDENCE_PLATFORM_H_
