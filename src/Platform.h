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
#ifndef SWORD_PLATFORM_H_
#define SWORD_PLATFORM_H_

#if defined(__WIN32__)|| defined (_WIN32)
#   define SWORD_PLANTFORM_WIN
#else
#   define SWORD_PLANTFORM_LINUX
#endif

#ifdef SWORD_PLATFORM_WIN
// BUILD_DLL was defined by VS ,or not
#   if defined BUILD_DLL
#       define SWORD_EXPORT  __declspec(dllexport)
#   else
#       define SWORD_EXPORT  __declspec(dllimport)
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

#endif //SWORD_PLATFORM_H_
