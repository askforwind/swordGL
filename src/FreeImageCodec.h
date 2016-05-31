/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: FreeImageCodec.h
#
# Last modified:2016-4-24
#
# Description:
#
#=====================================*/

#ifndef SWORD_FREEIMAGECODEC_H__
#define SWORD_FREEIMAGECODEC_H__

#include "Platform.h"
#include <stdint.h>
struct FIBITMAP;

SWORD_BEGIN

class SWORD_EXPORT FreeImageCodec{
public:
	FreeImageCodec();

	~FreeImageCodec();

	enum ColorType {
		RGB,
		RGBA,
		OTHER
	};

	struct Result {
		unsigned char* byte;
		uint32_t width;
		uint32_t height;
		uint8_t color_type;
	};

	Result decode(const char* filename);


private:
	void startUp();

	void shutDown();

	FIBITMAP* bitmap_;
};


SWORD_END


#endif // SWORD_FREEIMAGECODEC_H__