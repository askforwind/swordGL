/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-03-30 22:00
#
# Filename: shaderManager.h
#
# Description:
#
==================================================================*/

#ifndef SWORD_SHADERMANAGER_H
#define SWORD_SHADERMANAGER_H

#include"Platform.h"
#include<string>
#include<map>

class SWORD_EXPORT ShaderManager {
  public:
    bool cmpilerShader(const std::string& shader_path_);
  private:
    std::map <std::string, uint32_t> shader_list_;
};

#endif


