#include "Combiner.h"
#include "BufferManager.h"
#include "ResourceDef.h"
#include "Technique.h"
#include "config.h"
#include "Skeleton.h"
#include "ResourceGroup.h"
#include <algorithm>

SWORD_BEGIN

Combiner* Singleton<Combiner>::msSingleton = nullptr;

void Combiner::combineNewModule(ModulePtr module,TechniquePtr tech) {
	std::sort( module->sub_meshs.begin(),module->sub_meshs.end(),
			  [](std::string& mesh0_id,std::string& mesh1_id)->bool {
		MeshPtr m0 = ResourceGroup::instance().get<ResourceGroup::MESH>(mesh0_id);
		MeshPtr m1 = ResourceGroup::instance().get<ResourceGroup::MESH>(mesh1_id);
		return m0->material_id < m1->material_id;
	});

	for (size_t i = 0; i < module->sub_meshs.size(); i++) {
		
		combineNewMesh(ResourceGroup::instance().
					   get<ResourceGroup::MESH>(module->sub_meshs[i]), tech);
	}

}

void Combiner::combineNewMesh(MeshPtr mesh, TechniquePtr tech) {
	assert(mesh_offset_.find(mesh->self_id()) == mesh_offset_.end());
	VAO_id vao = mapToVAO(tech);
	
	VertexOffset offset = BufferManager::instance().
		map(vao, Attribute::INDEX, mesh->index.data(),
			mesh->index.size() * sizeof(mesh->index[0]));

	BufferManager::instance().enableIndex(vao);
	mesh_offset_.insert(std::make_pair(mesh->self_id(), offset / sizeof(mesh->index[0])));

	const Technique::AttributeInfo* layout = tech->get_attribute_layout();
	for (int i = 0; i != tech->get_attribute_layout_size(); ++i) {
		if (layout[i].location >= 0) {
			
			VertexDataInfo info = creatVertexDataInfo(layout[i].attr, mesh);
			BufferManager::instance().map(vao,
												  layout[i].attr,
												  info.data,
												  info.bsize);
			BufferManager::instance().enableVertexAttrib(vao, 
																 layout[i].attr,
																 layout[i].location);
		}
	}
}

VAO_id Combiner::mapToExistVAO(TechniquePtr tech) {
	auto iter = tech_list_.find(tech);
	return iter == tech_list_.end() ? kINVALID_VAO_ID : iter->second;
}

//void Combiner::creatDrawCallQueue(ModulePtr module,DrawCallVecRef dcv) {
//
//	auto iter = module_batch_info_.find(module->self_id);
//	assert(iter != module_batch_info_.end());
//
//	ModuleMeshBatchInfo& mbi = iter->second;
//
//	const Technique::UniformInfo* ulayout = mbi.tech->get_uniform_layout();
//	size_t ulayout_size = mbi.tech->get_uniform_layout_size();
//
//	VAO_id vao = mapToExistVAO(mbi.tech);
//	assert(vao != kINVALID_VAO_ID);
//	bool has_deal_with_skeleton = false;
//
//	dcv.reserve(mbi.different_material_signal.size());
//
//	for (size_t i = 0; i < mbi.different_material_signal.size(); i++) {
//		MeshPtr mesh = ResourceGroup::instance().
//			get<ResourceGroup::MESH>(module->sub_meshs[i]);
//
//		MaterialPtr material = ResourceGroup::instance().
//			get<ResourceGroup::MATERIAL>(mesh->material_id);
//		
//		DrawCall dc;
//		dc.tech_id = mbi.tech->self_id();
//		dc.vao = vao;
//		dc.offset = mbi.offset[i];
//		dc.index_count = mbi.offset[i + 1] - mbi.offset[i];
//
//		for (int k = 0; k < ulayout_size; ++k) {
//			
//			if (ulayout[k].location >= 0) {
//
//				Command cmd;
//				cmd.location = ulayout[k].location;
//
//				if (ulayout[k].target == UniformTarget::BONE_TRANSFORM && !has_deal_with_skeleton) {
//					has_deal_with_skeleton = true;
//
//					SkeletonPtr ske = ResourceGroup::instance().
//						get<ResourceGroup::SKELETON>(mesh->skeleton_id);
//
//					assert(ske != nullptr);
//
//					cmd.uniform_type = g_UniformTargetTypeMap[UniformTarget::BONE_TRANSFORM];
//					cmd.size = ske->get_effectiv_joints_nums();
//					cmd.location = ulayout[k].location;
//					cmd.value_ptr = &ske->getAnimationMatrix[0];
//				}
//				else {
//					initCommandValue(cmd, ulayout[i].target, material);
//				}
//
//				dc.command.push_back(cmd);
//
//			}
//		}
//
//		dcv.push_back(dc);
//	}
//
//}

VertexOffset Combiner::get_mesh_offset(const std::string & mesh_id) {
	auto iter = mesh_offset_.find(mesh_id);
	assert(iter != mesh_offset_.end());
	return iter == mesh_offset_.end() ? kINVALID_MAGIC_NUM : iter->second;
}

Combiner::VertexDataInfo Combiner::creatVertexDataInfo(int attr,MeshPtr mesh) {
	VertexDataInfo info;
	switch (attr) {
	case Attribute::POSITION:
		info.data = mesh->vertex.data();
		info.bsize = mesh->vertex.size() * sizeof(mesh->vertex[0]);
		break;
	case Attribute::NORMAL:
		info.data = mesh->normal.data();
		info.bsize = mesh->normal.size() * sizeof(mesh->normal[0]);
		break;
	case Attribute::TEXCOORD:
		info.data = mesh->texCoord.data();
		info.bsize = mesh->texCoord.size() * sizeof(mesh->texCoord[0]);
		break;
	case Attribute::BONE:
		info.data = mesh->joint_id.data();
		info.bsize = mesh->joint_id.size() * sizeof(mesh->joint_id[0]);
		break;
	case Attribute::WEIGHT:
		info.data = mesh->joint_weight.data();
		info.bsize = mesh->joint_weight.size() * sizeof(mesh->joint_weight[0]);
		break;
	default:assert(0); break;
	}
	return info;
}

VAO_id Combiner::mapToVAO(TechniquePtr tech) {
	auto iter = tech_list_.find(tech);
	if (iter == tech_list_.end()) {
		VAO_id vao = BufferManager::instance().creatVAO();
		iter = tech_list_.insert(std::make_pair(tech, vao)).first;
	}
	return iter->second;
}

SWORD_END