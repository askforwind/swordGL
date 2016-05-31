#include"FPSCamera.h"
#include"glm\gtx\quaternion.hpp"
#include<iostream>

SWORD_BEGIN

FPSCamera::FPSCamera()
	:Camera()
	, mouse_sensitivity_(1.0f){
}

void FPSCamera::moveMouse(float offset_x, float offset_y, float ration) {
	float horizontal_radian = offset_x*ration*mouse_sensitivity_;
	float vertical_radian = offset_y*ration*mouse_sensitivity_;
	
	Camera::pitch(vertical_radian);
	Camera::yaw(horizontal_radian);

}

SWORD_END