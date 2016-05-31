#ifndef SWORD_CAMERA_H__
#define SWORD_CAMERA_H__

#include "Platform.h"
#include <glm\glm.hpp>
SWORD_BEGIN

class SWORD_EXPORT Camera
{
public:
	Camera();
		
	~Camera() = default;

	void set_clip_plane(float near_clip, float far_clip);

	void set_aspect(float aspect);

	void lookAt(const glm::vec3& center);

	void set_position(const glm::vec3& p);

	void set_direction(const glm::vec3& d);

	void translate(const glm::vec3& v);

	void yaw( float radian);

	void pitch( float radian);

	glm::vec3 get_direction()const;

	glm::vec3 get_up()const;

	glm::vec3 get_right()const;

	const glm::vec3& get_position()const;

	const glm::mat4& get_matrix()const;

protected:
	void updateView();

	void updateProject();

	void updateValue();

	glm::mat4 projection_;
	glm::mat4 view_;
	glm::vec3 direction_;
	glm::vec3 position_;
	glm::vec3 up_;

	glm::mat4 matrix_;

	float view_filed_;
	float aspect_;
	float near_clip_;
	float far_clip_;
};

SWORD_END

#endif // SWORD_CAMERA_H__
