#include "Node.h"

SWORD_BEGIN

Node::Node()
	:world_(glm::mat4(1.0f))
	, local_(glm::mat4(1.0f)) {
}

glm::vec3 Node::get_position()const {
	glm::vec4 p = world_*local_*glm::vec4(0, 0, 0, 1);
	return glm::vec3(p.x, p.y, p.z);
}

glm::mat4 Node::get_model_matrix() const {
	return world_*local_;
}

void Node::set_position(const glm::vec3& pos, const Coord& c) {
	switch (c)
	{
	case Coord::World:local_ = world_ = glm::mat4(1.0f); world_[3] = glm::vec4(pos, 1); break;
	case Coord::Local:local_ = glm::mat4(1.0f); local_[3] = glm::vec4(pos, 1); break;
	}
}

void Node::translate(const glm::vec3& trans_vec, const Coord& c) {
	switch (c)
	{
	case Coord::World:world_ = glm::translate(world_, trans_vec); break;
	case Coord::Local:local_ = glm::translate(local_, trans_vec); break;
	}

}

void Node::rotate(const float& radians, const glm::vec3 & axis, const Coord& c) {
	switch (c)
	{
	case Coord::World:world_ = glm::rotate(world_, radians, axis); break;
	case Coord::Local:local_ = glm::rotate(local_, radians, axis); break;
	}
}

// rotate about z
void Node::yaw(const float& radians, const Coord& c) {
	switch (c)
	{
	case Coord::World:world_ = glm::rotate(world_, radians, glm::vec3(0, 0, 1)); break;
	case Coord::Local:local_ = glm::rotate(local_, radians, glm::vec3(0, 0, 1)); break;
	}
}
// rotate about y
void Node::pitch(const float& radians, const Coord& c) {
	switch (c)
	{
	case Coord::World:world_ = glm::rotate(world_, radians, glm::vec3(0, 1, 0)); break;
	case Coord::Local:local_ = glm::rotate(local_, radians, glm::vec3(0, 1, 0)); break;
	}
}
// rotate about x
void Node::roll(const float& radians, const Coord& c) {
	switch (c)
	{
	case Coord::World:world_ = glm::rotate(world_, radians, glm::vec3(1, 0, 0)); break;
	case Coord::Local:local_ = glm::rotate(local_, radians, glm::vec3(1, 0, 0)); break;
	}
}

SWORD_END
