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
	, far_clip_(100.f){
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
	glm::vec3 right = glm::cross(direction_, up_);
	up_ = glm::cross(right, direction_);
	up_ = glm::normalize(up_);
	direction_ = glm::normalize(direction_);
}

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

void Camera::forward(float unit) {
	position_ += unit*direction_;
	updateView();
}

void Camera::backward(float unit) {
	position_ -= unit*direction_;
	updateView();
}

void Camera::moveLeft(float unit) {
	position_ -= unit*glm::normalize(glm::cross(direction_, up_));
	updateView();
}

void Camera::moveRight(float unit) {
	position_ += unit*glm::normalize(glm::cross(direction_, up_));
	updateView();
}

void Camera::moveUp(float unit) {
	position_ -= unit* up_;
	updateView();
}

void Camera::moveDown(float unit) {
	position_ += unit* up_;
	updateView();
}

void Camera::translate(const glm::vec3& v) {
	position_ += v*direction_;
	updateView();
}

const glm::vec3 Camera::get_direction()const {
	return direction_;
}

SWORD_END