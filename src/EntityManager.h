/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: EntityManager.h
#
# Last modified:2016-5-11
#
# Description:
#
#=====================================*/
#ifndef SWORD_ENTITY_MANAGER_H__
#define SWORD_ENTITY_MANAGER_H__

#include "Platform.h"
#include "CommandDef.h"
#include <vector>
#include <unordered_map>
#include <string>

SWORD_BEGIN

class Entity;
class Camera;
class Combiner;

typedef std::vector<Entity> EntityVec;
typedef std::vector<Entity>& EntityVecRef;

class SWORD_EXPORT EntityManager {
public:
	EntityManager(Combiner* combiner, Camera* camera);
	~EntityManager() = default;

	Entity* creatEntity(TechniquePtr tech, ModulePtr module);

	void generateDrawCallQueue(MultiDrawCallCommandVecRef mdccv);
private:

	struct ModuleDrawCallInfo {
		std::vector<int> different_material_signal;
		std::vector<int> offset;
		SkeletonWeakPtr skeleton;
		int32_t slot;
		TechniqueWeakPtr tech;
	};

	void fillCommand(DrawCall& dc, TechniquePtr tech,  MeshPtr mesh);

	void fillLongLifeCycleCommand(LongLifeCycleCommandVec& llcmd, ModuleDrawCallInfo& mdci,Entity& entity);

	void generateEntityDrawCall(MultiDrawCallCommandRef mdcc, uint32_t slot);

	void generateBatchInfo(ModulePtr module, int32_t slot,TechniquePtr tech);

	Combiner* combiner_;

	Camera* camera_;
	
	// using for quick loop through
	std::vector<ModuleWeakPtr> module_slots_;
	std::vector<EntityVec> entity_list_;

	std::unordered_map<std::string, ModuleDrawCallInfo> module_batch_info_list_;
};


SWORD_END


#endif // SWORD_ENTITY_MANAGER_H__