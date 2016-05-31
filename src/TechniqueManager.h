/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: TechniqueManager.h
#
# Last modified:2016-5-7
#
# Description:
#
#=====================================*/

#ifndef SWORD_TECHNIQUEMANAGER_H__
#define SWORD_TECHNIQUEMANAGER_H__

#include "Singleton.h"
#include "TypeDef.h"
#include "Platform.h"
#include <unordered_map>


SWORD_BEGIN

class TechniqueManager:public Singleton<TechniqueManager> {
public:
	TechniqueManager() = default;
	~TechniqueManager() = default;

	TechniquePtr creatTechnique(const std::string& id);

	TechniquePtr get(const std::string& id);

private:
	bool findTechnique(const std::string& key);

	std::unordered_map<std::string, TechniquePtr> technique_list_;
};

SWORD_END

#endif // SWORD_TECHNIQUEMANAGER_H__
