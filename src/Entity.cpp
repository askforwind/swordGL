#include "Entity.h"
#include <glm\gtx\transform.hpp>
#include <glm/gtx/quaternion.hpp>
SWORD_BEGIN

Entity::Entity()
	:final_(1.0f)
{
	model_[0] = glm::mat4(1.0f);
	model_[1] = glm::mat4(1.0f);
	timer_.begin();
}



void Entity::set_pos(glm::vec3 & m,Coord c)
{
	model_[c] = glm::translate(glm::mat4(1.0f), m);
	updateModel();
}

glm::vec3 Entity::get_pos()
{
	return glm::vec3(final_[3]);
}

void Entity::translate(glm::vec3 & v, Coord c)
{
	model_[c] = glm::translate(model_[c], v);
	updateModel();
}

void Entity::rotate(glm::quat & q, Coord c)
{
	model_[c] = glm::toMat4(q)*model_[c];
	updateModel();
}

void Entity::updateModel()
{
	final_ = model_[Coord::WORLD] * model_[Coord::LOCAL];
}


SWORD_END
