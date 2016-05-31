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
#include <glm\gtx\quaternion.hpp>
#include <glm\gtx\transform.hpp>

SWORD_BEGIN

Camera::Camera()
	: direction_(glm::vec3(0, 0, -1))
	, position_(glm::vec3(0, 0, 5))
	, up_(0.0f, 1.0f, 0.0f)
	, view_filed_(glm::radians(45.0f))
	, aspect_(4.0f/3.0f)
	, near_clip_(1.0f)
	, far_clip_(1000.f)
	, matrix_(1.0f){
	updateProject();
	updateView();
}

void Camera::updateView() {
	view_ = glm::lookAt(position_, position_ + direction_, glm::vec3(0.0f, 1.0f, 0.0f));
	matrix_ = projection_*view_;
}
	
void Camera::updateProject() {
	projection_ = glm::perspective(view_filed_, aspect_, near_clip_, far_clip_);
}

void SWORD::Camera::updateValue() {
	glm::vec3 right = glm::cross(direction_, glm::vec3(0.0f, 1.0f, 0.0f));
	up_ = glm::cross(right, direction_);
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

const glm::mat4& Camera::get_matrix()const {
	return matrix_;
	 
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

void Camera::yaw(float radian) {
	radian /= 2.0f;
	glm::vec4 center(position_ + direction_, 1.0f);
	glm::quat q(cos(radian), sin(radian)*up_);
	set_direction(glm::rotate(q,direction_));
}

void Camera::pitch(float radian) {
	radian /= 2.0f;
	glm::vec4 center(position_ + direction_, 1.0f);
	glm::vec3 right = glm::cross(direction_, up_);
	glm::quat q(cos(radian), sin(radian)*right);
	set_direction(glm::rotate(q, direction_));
}


glm::vec3 Camera::get_direction()const {
	return direction_;
}

glm::vec3 Camera::get_up() const {
	return up_;
}


glm::vec3 Camera::get_right()const {
	return glm::cross(direction_, up_);
}

const glm::vec3& Camera::get_position() const {
	return position_;
}

SWORD_END