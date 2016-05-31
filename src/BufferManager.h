/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: BufferManager.h
#
# Last modified:2016-5-6
#
# Description:
#
#=====================================*/

#ifndef SWORD_BUFFERMANAGER_H__
#define SWORD_BUFFERMANAGER_H__
#include "Platform.h"
#include "singleton.h"
#include "CommandDef.h"
#include <map>

SWORD_BEGIN

class SWORD_EXPORT BufferManager:public Singleton<BufferManager> {
public:

	BufferManager() = default;
	~BufferManager();

	struct VBO {
		VBO();
		VBO_id id;
		Attribute type;
		size_t max_size;
		size_t valid_size;
	};

	uint32_t creatVAO();

	int map(VAO_id vao, Attribute used_for, void* data, size_t bsize);

	VBO getParticularTypeVbo(VAO_id vao, Attribute type);

	VBO& getParticularTypeVboRef(VAO_id vao, Attribute type);

	void enableVertexAttrib(VAO_id vao, Attribute vbo_type, int32_t location_in_shader);

	void enableIndex(VAO_id vao);
private:

	void updateIndex(uint32_t* p_buffer_data, uint32_t distance, uint32_t size);

	uint32_t getVertexNum(VAO_id vao);

	void copyVBOData(VBO& src, VBO& dst);

	void releaseVBO(VBO& vbo);

	void initBuffer(VAO_id vao,VBO& vbo, Attribute used_for);

	int _map(VAO_id vao,VBO& vbo, void* data, size_t bsize);

	void releaseAll();

	void releaseVAO(VAO_id vao);

	void resetVBOMemoeySize( VBO& vbo, uint32_t VertexInfoType, size_t size);

	std::multimap<VAO_id, VBO> buffer_list_;
};

SWORD_END

#endif // SWORD_BUFFERMANAGER_H__