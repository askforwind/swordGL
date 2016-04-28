/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: AssimpCodec.h
#
# Last modified:2016-4-24
#
# Description:
#
#=====================================*/

#ifndef SWORD_ASSIMPCODEC_H__
#define SWORD_ASSIMPCODEC_H__

#include "Platform.h"
#include "ResourceDef.h"
#include "Skeleton.h"
#include <vector>

SWORD_BEGIN

class AssimpCodecImp;

class SWORD_EXPORT AssimpCodec{
public:

	AssimpCodec();
	~AssimpCodec();

	void decode(const std::string& filename);
	void load(std::vector<Mesh>& mesh,
			  std::vector<Material>& material,
			  Skeleton& skeleton);
private:
	AssimpCodecImp* p_Imp_;
};


#endif // SWORD_ASSIMPCODEC_H__

SWORD_END