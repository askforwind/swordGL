/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: CommandDef.h
#
# Last modified:2016-5-10
#
# Description:
#
#=====================================*/
#ifndef SWORD_COMMANDDEF_H__
#define SWORD_COMMANDDEF_H__

#include <cstdint>
#include <string>
#include <vector>
#include "config.h"
#include "TypeDef.h"
namespace SWORD {

	enum Attribute {
		//---shader data and vbo type----
		POSITION ,
		NORMAL ,
		TEXCOORD ,
		BONE ,
		WEIGHT ,

		//------vbo type-----
		INDEX ,

		UNASSIGNED
	};

#define kSHADER_ATTRIBUTE_NUM 5

	typedef uint32_t VAO_id;
	typedef uint32_t VBO_id;
	typedef uint32_t VertexOffset;

	enum UniformTarget {
		DIFF_COLOR_MAP ,

		VP_MATRIX ,
		MODEL_MATRIX ,
		MVP_MATRIX ,
		VIEW_MATRIX,

		SHININESS,
		SPECULAR,
		DIFFUSE,

		BONE_TRANSFORM ,
		UNKNOWN_TARGET 
	};
#define kSHADER_UNIFORM_NUM 9

	enum UniformType {
		MAT,
		INT,
		FLOAT,
		VEC3,
		UNKNOWN_TYPE
	};

	static UniformType g_UniformTargetTypeMap[kSHADER_UNIFORM_NUM + 1] = {
		INT,MAT,MAT,MAT,MAT,FLOAT,VEC3,VEC3,VEC3,UNKNOWN_TYPE
	};

	struct TextureOp {
		TextureOp()
			:ptr(nullptr)
			, floor(-1) {
		}
		TexturePtr ptr;
		int8_t floor;
	};

	struct Command {
		Command()
			:target(UniformTarget::UNKNOWN_TARGET)
			, location(kINVALID_MAGIC_NUM)
			, size(kINVALID_MAGIC_NUM) {
			value.int_value = 0;
		}

		UniformTarget target;
		int32_t location;
		union _value {
			void* value_ptr;
			int32_t int_value;
			float float_value;
		}value;
		size_t size;
	};

	struct DrawCall {

		DrawCall& operator =(const DrawCall& dc) {
			this->vao = dc.vao;
			this->offset = dc.offset;
			this->index_count = dc.index_count;
			this->tex_op = dc.tex_op;
			this->command = dc.command;
			return *this;
		}

		DrawCall& operator = (DrawCall&& dc) {
			this->vao = dc.vao;
			this->offset = dc.offset;
			this->index_count = dc.index_count;
			this->tex_op = std::move(dc.tex_op);
			this->command = std::move(dc.command);
			return *this;
		}

		DrawCall(const DrawCall& dc) {
			this->vao = dc.vao;
			this->offset = dc.offset;
			this->index_count = dc.index_count;
			this->tex_op = dc.tex_op;
			this->command = dc.command;
		}

		DrawCall(DrawCall&& dc) {
			this->vao = dc.vao;
			this->offset = dc.offset;
			this->index_count = dc.index_count;
			this->tex_op = std::move(dc.tex_op);
			this->command = std::move(dc.command);
		}

		DrawCall()
			:vao(kINVALID_VAO_ID)
			, offset(kINVALID_MAGIC_NUM)
			, index_count(kINVALID_MAGIC_NUM) {
		}

		VAO_id vao;
		VertexOffset offset;
		uint32_t index_count;
	
		std::vector<Command> command;
		std::vector<TextureOp> tex_op;
	};

	typedef std::vector<DrawCall> DrawCallVec;
	typedef std::vector<DrawCall>& DrawCallVecRef;

	struct LongLifeCycleCommand {
		LongLifeCycleCommand()
			:target(UniformTarget::UNKNOWN_TARGET)
			, location(kINVALID_MAGIC_NUM) {
		}

		UniformTarget target;
		int32_t location;

		struct _Skeleton {
			Skeleton* ptr;
			float animation_time;
		};

		union _value {
			
			const float* data_ptr;
			_Skeleton skeleton;

		};

		_value value;
	};

	typedef std::vector<LongLifeCycleCommand> LongLifeCycleCommandVec;
	typedef LongLifeCycleCommandVec& LongLifeCycleCommandVecRef;

	struct MultiDrawCallCommand {

		MultiDrawCallCommand() = default;
		MultiDrawCallCommand(const MultiDrawCallCommand& cmd) {
			this->draw_calls = cmd.draw_calls;
			this->lone_life_cmds = cmd.lone_life_cmds;
			this->tech_ptr = cmd.tech_ptr;
		}

		MultiDrawCallCommand(MultiDrawCallCommand&& cmd) {
			this->draw_calls = std::move(cmd.draw_calls);
			this->lone_life_cmds = std::move(cmd.lone_life_cmds);
			this->tech_ptr = cmd.tech_ptr;
		}

		MultiDrawCallCommand& operator=(MultiDrawCallCommand& cmd) {
			this->draw_calls = cmd.draw_calls;
			this->lone_life_cmds = cmd.lone_life_cmds;
			this->tech_ptr = cmd.tech_ptr;
		}
		MultiDrawCallCommand& operator=(MultiDrawCallCommand&& cmd) {
			this->draw_calls = std::move(cmd.draw_calls);
			this->lone_life_cmds = std::move(cmd.lone_life_cmds);
			this->tech_ptr = cmd.tech_ptr;
		}

		LongLifeCycleCommandVec lone_life_cmds;
		TechniquePtr tech_ptr;
		DrawCallVec draw_calls;
	};

	typedef  std::vector<MultiDrawCallCommand> MultiDrawCallCommandVec;
	typedef	 std::vector<MultiDrawCallCommand>& MultiDrawCallCommandVecRef;
	typedef  MultiDrawCallCommand& MultiDrawCallCommandRef;

}

#endif // SWORD_COMMANDDEF_H__