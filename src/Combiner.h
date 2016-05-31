
/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: Combiner.h
#
# Last modified:2016-5-6
#
# Description:
#
#=====================================*/

#ifndef SWORD_Combiner_H__
#define SWORD_Combiner_H__

#include "singleton.h"
#include "CommandDef.h"
#include <unordered_map>

SWORD_BEGIN

class SWORD_EXPORT Combiner:public Singleton<Combiner> {
public:
	
	Combiner() = default;
	
	void combineNewModule(ModulePtr module, TechniquePtr tech);

	VAO_id mapToExistVAO(TechniquePtr tech);

	VertexOffset get_mesh_offset(const std::string & mesh_id);

private:

	void combineNewMesh(MeshPtr mesh, TechniquePtr tech);

	struct VertexDataInfo {
		size_t bsize;
		void* data;
	};

	VertexDataInfo creatVertexDataInfo(int attr,MeshPtr mesh);

	VAO_id mapToVAO(TechniquePtr tech);

	std::unordered_map<TechniquePtr, VAO_id> tech_list_;
	std::unordered_map<std::string, VertexOffset> mesh_offset_;
};

SWORD_END

#endif;
