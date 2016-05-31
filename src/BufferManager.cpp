#include "BufferManager.h"
#include "config.h"
#include "LogManager.h"
#include "GL/glew.h"
#include <cassert>
SWORD_BEGIN

BufferManager* Singleton<BufferManager>::msSingleton = nullptr;

BufferManager::VBO::VBO()
	:id(kINVALID_VBO_ID)
	, type(Attribute::UNASSIGNED)
	, max_size(0)
	, valid_size(0) {
}

void BufferManager::releaseVAO(VAO_id vao) {

	auto range = buffer_list_.equal_range(vao);

	for (auto iter = range.first; iter != range.second; ++iter) {
		CHECK_GL_ERROR(glDeleteBuffers(1, &iter->second.id));
	}

	CHECK_GL_ERROR(glDeleteVertexArrays(1, &vao));

	buffer_list_.erase(range.first, range.second);
}

BufferManager::~BufferManager() {
	releaseAll();
}

uint32_t BufferManager::creatVAO() {
	VAO_id vao;
	CHECK_GL_ERROR(glGenVertexArrays(1, &vao));
	assert(buffer_list_.find(vao) == buffer_list_.end());
	buffer_list_.insert(std::make_pair(vao, VBO()));
	return vao;
}

void BufferManager::releaseVBO(VBO & vbo) {
	glDeleteBuffers(1, &vbo.id);
	vbo = VBO();
}

void BufferManager::initBuffer(VAO_id vao, VBO & vbo, Attribute type) {

	CHECK_GL_ERROR(glBindVertexArray(vao));

	if (vbo.id != kINVALID_VBO_ID)
		CHECK_GL_ERROR(glDeleteBuffers(1, &vbo.id));

	vbo.type = type;
	CHECK_GL_ERROR(glGenBuffers(1, &vbo.id));
	resetVBOMemoeySize(vbo, type == INDEX
					   ? GL_ELEMENT_ARRAY_BUFFER
					   : GL_ARRAY_BUFFER, 1200);

	CHECK_GL_ERROR(glBindVertexArray(0));
}

int BufferManager::map(VAO_id vao, Attribute vbo_type,
						void * data, size_t bsize) {
	auto range = buffer_list_.equal_range(vao);
	assert(range.first != buffer_list_.end());

	VBO* p_vbo = nullptr;
	for (auto iter = range.first; iter != range.second; iter++) {
		if (iter->second.type == vbo_type
			||iter->second.type== UNASSIGNED) {
			p_vbo = &iter->second;
			break;
		}
	}

	if (p_vbo == nullptr) {
		VBO temp;
		initBuffer(vao, temp, vbo_type);
		auto iter = buffer_list_.insert(std::make_pair(vao, temp));
		p_vbo = &iter->second;
	}
	else if (p_vbo->type == UNASSIGNED) {
		assert(p_vbo->id == kINVALID_VBO_ID);
		initBuffer(vao, *p_vbo, vbo_type);
	}

	return _map(vao, *p_vbo, data, bsize);
}

BufferManager::VBO BufferManager::getParticularTypeVbo(VAO_id vao, Attribute type) {
	return getParticularTypeVboRef(vao, type);
}

BufferManager::VBO & BufferManager::getParticularTypeVboRef(VAO_id vao, Attribute type) {
	auto range = buffer_list_.equal_range(vao);

	for (auto iter = range.first; iter != range.second; ++iter) {
		if (iter->second.type == type)
			return iter->second;
	}
	assert(0);
	return VBO();
}

namespace {
	std::pair<int,GLenum> matchToGLType(Attribute type) {
		GLenum gtype = 0;
		int size = 0;
		switch (type) {
		case SWORD::Attribute::POSITION:
			gtype = GL_FLOAT;
			size = 3;
			break;
		case SWORD::Attribute::NORMAL:
			gtype = GL_FLOAT;
			size = 3;
			break;
		case SWORD::Attribute::TEXCOORD:
			gtype = GL_FLOAT;
			size = 2;
			break;
		case SWORD::Attribute::BONE:
			gtype = GL_INT;
			size = 4;
			break;
		case SWORD::Attribute::WEIGHT:
			gtype = GL_FLOAT;
			size = 4;
			break;
		default:
			assert(0);
			break;
		}
		return std::make_pair(size, gtype);
	}

	bool needIntType(SWORD::Attribute type) {
		return type == SWORD::Attribute::BONE;
	}

}

void BufferManager::enableVertexAttrib(VAO_id vao, Attribute vbo_type, int32_t location_in_shader) {

	assert(location_in_shader != kINVALID_SHADER_LOCATION);
	VBO& vbo = getParticularTypeVboRef(vao, vbo_type);

	CHECK_GL_ERROR(glBindVertexArray(vao));
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vbo.id));
	CHECK_GL_ERROR(glEnableVertexAttribArray(location_in_shader));
	std::pair<int, GLenum> data_info = matchToGLType(vbo_type);
	if (needIntType(vbo_type) ){
		CHECK_GL_ERROR(glVertexAttribIPointer(location_in_shader, data_info.first,
					   data_info.second, 0, 0));
	}
	else {
		CHECK_GL_ERROR(glVertexAttribPointer(location_in_shader, data_info.first,
					   data_info.second, GL_FALSE, 0, 0));
	}
	CHECK_GL_ERROR(glBindVertexArray(0));
}

void BufferManager::enableIndex(VAO_id vao) {
	auto range = buffer_list_.equal_range(vao);
	for (auto iter  = range.first;  iter!= range.second; iter++) {
		if (iter->second.type == INDEX) {
			glBindVertexArray(vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iter->second.id);
			glBindVertexArray(0);
			return;
		}
	}
	assert(0);
}



void BufferManager::resetVBOMemoeySize(VBO & vbo, uint32_t VertexInfoType, size_t size) {
	assert(vbo.id != kINVALID_VBO_ID);
	CHECK_GL_ERROR(glBindBuffer(VertexInfoType, vbo.id));
	CHECK_GL_ERROR(glBufferData(VertexInfoType, size, NULL, GL_STATIC_DRAW));
	vbo.valid_size = 0;
	vbo.max_size = size;
	CHECK_GL_ERROR(glBindBuffer(VertexInfoType, 0));
}

void BufferManager::updateIndex(uint32_t * p_buffer_data, 
								uint32_t distance, 
								uint32_t size) {
	for (uint32_t i = 0; i < size; i++) {
		p_buffer_data[i] += distance;
	}
}

uint32_t BufferManager::getVertexNum(VAO_id vao) {
	auto range = buffer_list_.equal_range(vao);

	for (auto iter = range.first; iter != range.second; ++iter) {
		if (iter->second.type == Attribute::POSITION) {
			assert(iter->second.valid_size / (3 * sizeof(float))
				   * 3 * sizeof(float) == iter->second.valid_size);
			return iter->second.valid_size / (3 * sizeof(float));
		}
	}
	return 0;
}

void BufferManager::copyVBOData(VBO & src, VBO & dst) {
	assert(dst.max_size >= src.max_size);
	CHECK_GL_ERROR(glBindBuffer(GL_COPY_READ_BUFFER, src.id));
	CHECK_GL_ERROR(glBindBuffer(GL_COPY_WRITE_BUFFER, dst.id));
	CHECK_GL_ERROR(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, src.valid_size));
	dst.valid_size = src.valid_size;

	CHECK_GL_ERROR(glBindBuffer(GL_COPY_READ_BUFFER, 0));
	CHECK_GL_ERROR(glBindBuffer(GL_COPY_WRITE_BUFFER, 0));

}

int BufferManager::_map(VAO_id vao, VBO& vbo, void* data, size_t bsize) {
	if (vbo.valid_size + bsize > vbo.max_size) {
		VBO new_vbo;
		initBuffer(vao, new_vbo, vbo.type);
		
		size_t new_size = static_cast<size_t>(vbo.valid_size + bsize + 2100);
		resetVBOMemoeySize(new_vbo, vbo.type == INDEX
						   ? GL_ELEMENT_ARRAY_BUFFER
						   : GL_ARRAY_BUFFER,
						   new_size);
		
		copyVBOData(vbo, new_vbo);
		std::swap(vbo, new_vbo);

		releaseVBO(new_vbo);
	}

	void* ptr = nullptr;
	CHECK_GL_ERROR(glBindBuffer(GL_COPY_WRITE_BUFFER, vbo.id));
	CHECK_GL_ERROR(ptr = glMapBufferRange(GL_COPY_WRITE_BUFFER, vbo.valid_size,
				   bsize, GL_MAP_WRITE_BIT));

	memcpy_s(ptr, bsize, data, bsize);

	if (vbo.type == INDEX) {
		assert(!(bsize & 3));
		updateIndex(static_cast<uint32_t*>(ptr), 
					getVertexNum(vao),
					bsize / sizeof(uint32_t));
	}
	CHECK_GL_ERROR(glUnmapBuffer(GL_COPY_WRITE_BUFFER));
	CHECK_GL_ERROR(glBindBuffer(GL_COPY_WRITE_BUFFER, 0));

	vbo.valid_size += bsize;
	return vbo.valid_size - bsize;
}

void BufferManager::releaseAll() {
	auto iter = buffer_list_.begin();
	VAO_id last = iter->first;
	for (; iter != buffer_list_.end(); ++iter) {
		CHECK_GL_ERROR(glDeleteBuffers(1, &iter->second.id));
		if (iter->first != last) {
			CHECK_GL_ERROR(glDeleteVertexArrays(1, &last));
			last = iter->first;
		}
	}
	buffer_list_.clear();
}

SWORD_END