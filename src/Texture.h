/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-04-024 22:45
#
# Filename: Texture.h
#
# Description:
#
==================================================================*/
#ifndef SWORD_TEXTURE_H_
#define SWORD_TEXTURE_H_

#include<stdint.h>
#include"Platform.h"
#include <string>

SWORD_BEGIN

class ResourceGroup;

class SWORD_EXPORT Texture {
public:

	bool load(const std::string& filename,
			  bool compress = true,
			  bool auto_create_mipmap = true,
			  uint8_t mipmap_num = default_mipmap_num_);

	void unload();

	void setMipmapAuto(uint8_t max_level,
					   uint8_t start_level = 1);

	void bindToActiveUnit(uint8_t idx)const;

	//void setMipmapManualBegin();

	//void setMipMapManualEnd();
	friend class ResourceGroup;

	const static uint8_t default_mipmap_num_;

	void set_self_id(const std::string& id) { self_id_ = id; }

	const std::string& self_id()const { return self_id_; }

private:

	Texture();

	void createTexture(uint8_t* data,
					   uint8_t color_type,
					   bool compress,
					   bool create_mipmap,
					   uint8_t mipmap_num);

	std::string self_id_;

	uint32_t tex_id_;
	uint16_t width_;
	uint16_t height_;
	uint32_t inte_format_;
	uint32_t texture_format_;
};


SWORD_END

#endif
