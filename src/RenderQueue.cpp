#include "RenderQueue.h"
#include "GL/glew.h"
#include "LogManager.h"
#include "Technique.h"
#include "Skeleton.h"
#include "Texture.h"
#include "ResourceGroup.h"
SWORD_BEGIN

void RenderQueue::inputMultiDrawCall(const MultiDrawCallCommand& mdc) {
	render_queue_.push_back(mdc);
	assert(0);//using move to increase speed,never using this function
}

void RenderQueue::inputMultiDrawCall(MultiDrawCallCommand&& mdc) {
	render_queue_.push_back(std::move(mdc));
}

uint32_t RenderQueue::render()const {
	uint32_t num_of_draw_call = 0;
	for (size_t i=0;i<render_queue_.size();++i)
	{
		const MultiDrawCallCommand& mdc = render_queue_[i];
		
		mdc.tech_ptr->enable();
		
		dealWithLongLifeCycleCommds(mdc.lone_life_cmds,mdc.tech_ptr);

		for (size_t iDC = 0; iDC != render_queue_[i].draw_calls.size(); ++iDC) {

			const DrawCall& dc = mdc.draw_calls[iDC];

			dealWithCommand(dc, mdc.tech_ptr);
			dealWithTexture(dc);

			CHECK_GL_ERROR(glBindVertexArray(dc.vao));
			CHECK_GL_ERROR(glDrawElementsBaseVertex(GL_TRIANGLES, dc.index_count, GL_UNSIGNED_INT,
				(void*)(sizeof(uint32_t)*dc.offset), 0));
			num_of_draw_call++;
		}
	}
	CHECK_GL_ERROR(glBindVertexArray(0));
	return num_of_draw_call;
}

void RenderQueue::cleanDrawCallQueue() {
	render_queue_.clear();
}

void RenderQueue::dealWithLongLifeCycleCommds(const LongLifeCycleCommandVec& lone_cycle_cmds,
											  const TechniquePtr& tech)const {
	for (size_t i = 0; i != lone_cycle_cmds.size(); ++i) {
		const LongLifeCycleCommand& lccmd = lone_cycle_cmds[i];
		
		assert(lccmd.location != kINVALID_SHADER_LOCATION);
		assert(lccmd.target != UniformTarget::UNKNOWN_TARGET);

		switch (lccmd.target) {
		case UniformTarget::BONE_TRANSFORM:
			lccmd.value.skeleton.ptr->updateAnimation(lccmd.value.skeleton.animation_time);
			tech->set_multi_uniform_mat(lccmd.location,
										lccmd.value.skeleton.ptr->get_effectiv_joints_nums(),
										lccmd.value.skeleton.ptr->getAnimationMatrix()[0]);
			break;
		case UniformTarget::VP_MATRIX:
			tech->set_uniform_mat(lccmd.location, lccmd.value.data_ptr);
			break;
		case UniformTarget::MODEL_MATRIX:
			tech->set_uniform_mat(lccmd.location, lccmd.value.data_ptr);
			break;
		case UniformTarget::VIEW_MATRIX:
			tech->set_uniform_vec3(lccmd.location, lccmd.value.data_ptr);
			break;
		default:
			assert(0);
			break;
		}
	}
}

void RenderQueue::dealWithCommand(const DrawCall & dc,const TechniquePtr& tech)const {

	for (size_t i = 0; i != dc.command.size(); ++i) {

		const Command& cmd = dc.command[i];

		assert(cmd.location != kINVALID_SHADER_LOCATION);
		assert(cmd.size != kINVALID_MAGIC_NUM);
		assert(cmd.target != UniformTarget::UNKNOWN_TARGET);

		UniformType uniform_type = g_UniformTargetTypeMap[cmd.target];

		switch (uniform_type) {
		case UniformType::MAT:
			assert(cmd.value.value_ptr != nullptr);
			tech->set_multi_uniform_mat(cmd.location,
											cmd.size,
											static_cast<float*>(cmd.value.value_ptr));
			break;
		case UniformType::FLOAT:
			tech->set_uniform_float(cmd.location, cmd.value.float_value);
			break;
		case UniformType::INT:
			tech->set_uniform_int(cmd.location, cmd.value.int_value);
			break;
		case UniformType::VEC3:
			assert(cmd.value.value_ptr != nullptr);
			tech->set_uniform_vec3(cmd.location, static_cast<float*>(cmd.value.value_ptr));
			break;
		default:
			assert(0);
			break;
		}
	}
}

void RenderQueue::dealWithTexture(const DrawCall& dc)const {
	for (size_t i = 0; i != dc.tex_op.size(); ++i)
		dc.tex_op[i].ptr->bindToActiveUnit(dc.tex_op[i].floor);
}

size_t RenderQueue::get_render_queue_size() const {
	return render_queue_.size();
}


SWORD_END
