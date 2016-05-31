/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: RenderQueue.h
#
# Last modified:2016-5-10
#
# Description:
#
#=====================================*/

#ifndef SWORD_RENDERQUEUE_H__
#define SWORD_RENDERQUEUE_H__

#include "Platform.h"
#include "CommandDef.h"
SWORD_BEGIN

class SWORD_EXPORT RenderQueue {
public:

	void inputMultiDrawCall(const MultiDrawCallCommand& mdc);

	void inputMultiDrawCall(MultiDrawCallCommand&& mdc);

	uint32_t render()const;

	void cleanDrawCallQueue();

	void dealWithCommand(const DrawCall& dc,const TechniquePtr& tech)const;

	void dealWithTexture(const DrawCall& dc)const;

	void dealWithLongLifeCycleCommds( const LongLifeCycleCommandVec& cmds,const TechniquePtr& tech)const;

	size_t get_render_queue_size()const;
private:
	std::vector<MultiDrawCallCommand> render_queue_;
};

SWORD_END

#endif // SWORD_RENDERQUEUE_H__
