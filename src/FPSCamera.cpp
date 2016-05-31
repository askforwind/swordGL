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
	
	//direction_ = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::quat qh(cos(horizontal_radian / 2.0f), sin(horizontal_radian / 2.0f)*up_);
	direction_ = glm::rotate(qh, direction_);

	glm::vec3 h_axis = glm::cross(direction_, up_);
	glm::quat qv(cos(vertical_radian / 2.0f), sin(vertical_radian / 2.0f)*h_axis);
	direction_ = glm::rotate(qv, direction_);

	updateValue();
	updateView();
	//std::cout << direction_.x << " " << direction_.y << " " << direction_.z << std::endl;
	//std::cout << glm::degrees(horizontal_angle_) << std::endl << std::endl;
	/*Camera::set_direction(
		glm::vec3(cos(vertical_angle_) * sin(horizontal_angle_),
		sin(vertical_angle_),
		cos(vertical_angle_) * cos(horizontal_angle_)));

	glm::vec3 right = glm::vec3(
		sin(horizontal_angle_ - 3.14f / 2.0f),
		0,
		cos(horizontal_angle_ - 3.14f / 2.0f)
	);

	Camera::set_up(glm::cross(right, Camera::get_direction()));*/
}

SWORD_END