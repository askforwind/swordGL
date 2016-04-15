#include"FPSCamera.h"

SWORD_BEGIN

FPSCamera::FPSCamera()
	:Camera()
	, mouse_sensitivity_(1)
	, horizontal_angle_(3.1415926f)
	, vertical_angle_(0.0f) {
}

void FPSCamera::moveMouse(float offset_x, float offset_y, float ration) {
	horizontal_angle_ += offset_x*ration;
	vertical_angle_ += offset_y*ration;
	Camera::set_direction(
		glm::vec3(cos(vertical_angle_) * sin(horizontal_angle_),
		sin(vertical_angle_),
		cos(vertical_angle_) * cos(horizontal_angle_)));
}

SWORD_END