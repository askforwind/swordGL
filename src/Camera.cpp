/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: Camera.cpp
#
# Last modified:2016-4-13
#
# Description:
#
#=====================================*/



#include "Camera.h"

SWORD_BEGIN

Camera::Camera()
	: direction_(glm::vec3(0, 0, -1))
	, position_(glm::vec3(0, 0, 5))
	, up_(0.0f, 1.0f, 0.0f)
	, view_filed_(glm::radians(45.0f))
	, aspect_(4.0f/3.0f)
	, near_clip_(0.1f)
	, far_clip_(10000.f){
	updateProject();
	updateView();
}

void Camera::updateView() {
	view_ = glm::lookAt(position_, position_ + direction_, up_);
}
	
void Camera::updateProject() {
	projection_ = glm::perspective(view_filed_, aspect_, near_clip_, far_clip_);
}

void SWORD::Camera::updateValue() {
	//glm::vec3 right = glm::cross(direction_, up_);
	//up_ = glm::cross(right, direction_);
	up_ = glm::normalize(up_);
	direction_ = glm::normalize(direction_);
}

//void Camera::set_up(const glm::vec3& u) {
//	this->up_ = u;
//	updateValue();
//	updateView();
//}

void Camera::set_clip_plane(float near_clip, float far_clip) {
	this->near_clip_ = near_clip;
	this->far_clip_ = far_clip;
	updateProject();
}

void Camera::set_aspect(float aspect) {
	this->aspect_ = aspect;
	updateProject();
}

void Camera::lookAt(const glm::vec3& center) {
	this->direction_ = center - position_;
	updateValue();
	updateView();
}

void Camera::set_position(const glm::vec3& p) {
	this->position_ = p;
	updateView();
}

const glm::mat4 Camera::get_matrix() const {
	return  projection_*view_;
}

void Camera::set_direction(const glm::vec3& d) {
	this->direction_ = d;
	updateValue();
	updateView();
}


void Camera::translate(const glm::vec3& v) {
	position_ += v;
	updateView();
}

void Camera::yaw(const float radian) {
	glm::vec4 center(position_ + direction_, 1.0f);
	glm::mat4 r = glm::rotate(radian, glm::vec3(0.0f, 1.0f, 0.0f));
	center = r*center;
	lookAt(glm::vec3(center));
}

void Camera::pitch(const float radian) {
	glm::vec4 center(position_ + direction_, 1.0f);
	glm::mat4 r = glm::rotate(radian, glm::vec3(1.0f, 0.0f, 0.0f));
	center = r*center;
	lookAt(glm::vec3(center));
}


glm::vec3 Camera::get_direction()const {
	return direction_;
}

glm::vec3 Camera::get_up()const {
	return up_;
}

glm::vec3 Camera::get_right()const {
	return glm::cross(direction_, up_);
}

SWORD_END