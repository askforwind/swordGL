#ifndef SWORD_CAMERA_H__
#define SWORD_CAMERA_H__

#include "Node.h"

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

	void forward(float unit);

	void backward(float unit);

	void moveLeft(float unit);

	void moveRight(float unit);

	void moveUp(float unit);

	void moveDown(float unit);

	void translate(const glm::vec3& v);

	const glm::vec3 get_direction()const;

	const glm::mat4 get_matrix()const;

private:
	void updateView();

	void updateProject();

	void updateValue();

	glm::mat4 projection_;
	glm::mat4 view_;
	glm::vec3 direction_;
	glm::vec3 position_;
	glm::vec3 up_;

	float view_filed_;
	float aspect_;
	float near_clip_;
	float far_clip_;
};

SWORD_END

#endif // SWORD_CAMERA_H__
