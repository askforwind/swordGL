#include "EntityManager.h"
#include "ResourceDef.h"
#include "Technique.h"
#include "Entity.h"
#include "Combiner.h"
#include "ResourceGroup.h"
#include "LogManager.h"
#include "Camera.h"
SWORD_BEGIN



EntityManager::EntityManager(Combiner * combiner, Camera * camera) 
	:combiner_(combiner)
	,camera_(camera){
}

Entity*
EntityManager::creatEntity(TechniquePtr tech,
							ModulePtr module) {
	auto iter = module_batch_info_list_.find(module->self_id());

	if (iter == module_batch_info_list_.end()) {
		module_slots_.push_back(module);
		entity_list_.push_back(EntityVec());
		entity_list_.back().push_back(Entity());
		combiner_->combineNewModule(module, tech);
		generateBatchInfo(module, module_slots_.size() - 1,tech);
		return &entity_list_.back().back();
	}

	entity_list_[iter->second.slot].push_back(Entity());
	return &entity_list_[iter->second.slot].back();
}

void EntityManager::generateDrawCallQueue(MultiDrawCallCommandVecRef mdccvr) {

	mdccvr.reserve(module_slots_.size());

	for (size_t i = 0; i != module_slots_.size(); ++i) {
		MultiDrawCallCommand mdcc;
		generateEntityDrawCall(mdcc, i);
		mdccvr.push_back(std::move(mdcc));
	}
}

void EntityManager::generateEntityDrawCall(MultiDrawCallCommandRef mdcc, uint32_t slot) {
	ModuleWeakPtr module = module_slots_[slot];
	EntityVecRef entity_vec = entity_list_[slot];

	auto iter = module_batch_info_list_.find(module.lock()->self_id());
	assert(iter != module_batch_info_list_.end());
	ModuleDrawCallInfo& mdci = iter->second;

	mdcc.draw_calls.reserve(mdci.different_material_signal.size());
	mdcc.lone_life_cmds.reserve(5);

	// TODO multi entity,only one now;
	mdcc.tech_ptr = mdci.tech.lock();
	VAO_id vao = combiner_->mapToExistVAO(mdcc.tech_ptr);
	// TODO let vao store in long cycle command
	for (size_t i=0;i!=mdci.different_material_signal.size();++i)
	{
		DrawCall dc;
		dc.offset = mdci.offset[i];
		dc.index_count = mdci.offset[i + 1] - mdci.offset[i];
		dc.vao = vao;
		
		int mesh_idx = mdci.different_material_signal[i];

		fillCommand(dc, mdcc.tech_ptr, getMeshPtr(module.lock()->sub_meshs[mesh_idx]));

		mdcc.draw_calls.push_back(std::move(dc));
	}
	
	fillLongLifeCycleCommand(mdcc.lone_life_cmds, mdci, entity_vec[0]);

}

void EntityManager::fillCommand(DrawCall & dc, TechniquePtr tech,
								 MeshPtr mesh) {

	const Technique::UniformInfo* ulayout = tech->get_uniform_layout();
	size_t layout_size = tech->get_uniform_layout_size();

	MaterialPtr material = getMaterialPtr(mesh->material_id);
	uint8_t texture_floor = 0;

	for (size_t i=0;i!=layout_size;++i)
	{
		if (ulayout[i].location >= 0) {

			bool enable = false;
			Command cmd;
			cmd.location = ulayout[i].location;
			cmd.target = ulayout[i].target;
			cmd.size = 1;

			switch (ulayout[i].target) {
			case DIFF_COLOR_MAP:
				if (material->tex_diff.empty()) break;
				
				enable = true;
				dc.tex_op.push_back(TextureOp());
				dc.tex_op.back().floor = texture_floor;
				dc.tex_op.back().ptr = getDiffuseMapPtr(material);
				assert(dc.tex_op.back().ptr != nullptr);
				cmd.value.int_value = texture_floor;
				texture_floor++;
				break;
			case SHININESS:
				enable = true;
				cmd.value.float_value = material->shininess;
				break;
			case DIFFUSE:
				enable = true;
				cmd.value.value_ptr = &material->diffuse;
				break;
			case SPECULAR:
				enable = true;
				cmd.value.value_ptr = &material->specular;
				break;
			case MODEL_MATRIX:
			case VP_MATRIX:
			case BONE_TRANSFORM:
			case VIEW_MATRIX:
				break;

			default:
				assert(0);
				break;
			}

			if (enable)
				dc.command.push_back(std::move(cmd));
		}
	}
}

void EntityManager::fillLongLifeCycleCommand(LongLifeCycleCommandVec & llcmdvec,
											 ModuleDrawCallInfo & mdci,Entity& entity) {

	TechniquePtr tech = mdci.tech.lock();

	const Technique::UniformInfo* ulayout = tech->get_uniform_layout();
	size_t layout_size = tech->get_uniform_layout_size();

	for (size_t i = 0; i != layout_size; ++i) {
		if (ulayout[i].location >= 0) {

			LongLifeCycleCommand llcmd;
			llcmd.location = ulayout[i].location;
			llcmd.target = ulayout[i].target;

			bool enable = false;

			switch (ulayout[i].target) {
			case DIFF_COLOR_MAP:break;
			case MODEL_MATRIX:enable = true;
				llcmd.value.data_ptr = &entity.get_module_matrix()[0][0];
				break;
			case VP_MATRIX:enable = true;
				llcmd.value.data_ptr =&camera_->get_matrix()[0][0];
				break;
			case BONE_TRANSFORM:enable = true;
				llcmd.value.skeleton.ptr = mdci.skeleton.lock().get();
				assert(llcmd.value.skeleton.ptr != nullptr);
				llcmd.value.skeleton.animation_time = entity.get_animation_time();
				break;
			case VIEW_MATRIX:enable = true;
				llcmd.value.data_ptr = &camera_->get_position()[0];
				break;

			case SHININESS:
			case DIFFUSE:
			case SPECULAR:
				break;
			default:
				assert(0);
				break;
			}

			if (enable)
				llcmdvec.push_back(std::move(llcmd));
		}
	}

}



void EntityManager::generateBatchInfo(ModulePtr module,int32_t slot,TechniquePtr tech) {

	ModuleDrawCallInfo mdci;
	std::string last_material_id = "";
	mdci.skeleton = getSkeletonPtr(getMeshPtr(module->sub_meshs.front()));
	mdci.slot = slot;
	mdci.tech = tech;

	for (size_t i=0;i!=module->sub_meshs.size();++i)
	{
		std::string& mesh_id = module->sub_meshs[i];
		std::string& material_id = getMaterialId(mesh_id);
		if (material_id != last_material_id) {
			mdci.different_material_signal.push_back(i);
			uint32_t offset = combiner_->get_mesh_offset(mesh_id);
			mdci.offset.push_back(offset);
			last_material_id = material_id;
		}
	}

	// we can use offset[i+1] - offset[i] to calculate the index count;
	uint32_t last_offset = combiner_->get_mesh_offset(module->sub_meshs.back());
	uint32_t last_mesh_index_size = getMeshPtr(module->sub_meshs.back())->index.size();
	uint32_t last_position_in_vbo = last_offset + last_mesh_index_size;
	mdci.offset.push_back(last_position_in_vbo);
	module_batch_info_list_.insert(std::make_pair(module->self_id(), mdci));

	WIND_LOG_TRACE(DEFAULT_WIND_LOGGER, module->self_id() + " has combined");

}

SWORD_END