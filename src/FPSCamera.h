/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: FPSCamera.h
#
# Last modified:2016-4-15
#
# Description:
#
#=====================================*/

#ifndef SWORD_FPSCAMERA_H__
#define SWORD_FPSCAMERA_H__

#include "Camera.h"

SWORD_BEGIN

class SWORD_EXPORT FPSCamera :public Camera{
public:
	FPSCamera();
	~FPSCamera() = default;

	void set_mouse_sensitivity(float s) { 
		mouse_sensitivity_ = s; }

	void moveMouse(float offset_x, float offset_y, float ration);

private:
	float mouse_sensitivity_;
};

SWORD_END

#endif // SWORD_FPSCAMERA_H__