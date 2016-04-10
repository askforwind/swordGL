/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-02-17 22:25
#
# Filename: Config.h
#
# Description: 
#
==================================================================*/
#ifndef WIND7_CONFIG_H_ 
#define WIND7_CONFIG_H_

#include "Platform.h"

#   if defined(_DEBUG) || defined (DEBUG)
#       define WIND_DEBUG 1
#   else
#       define WIND_DEBUG 0
#   endif 

#   define WIND_EXPORT SWORD_EXPORT

#endif /*WIN7_CONFIG_H_*/
