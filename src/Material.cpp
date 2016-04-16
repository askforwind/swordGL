#include "Material.h"


SWORD::Material::
Material(){
	info_.diffuse = glm::vec3(0.0f);
	info_.specular = glm::vec3(0.0f);
	info_.ambient = glm::vec3(1.0f);
	info_.transparent = glm::vec3(1.0f);
	info_.wireframe = false;
	info_.twosided = false;
	info_.texture_id[0] = "";
	info_.texture_id[1] = "";
	info_.texture_id[2] = "";
	info_.texture_id[3] = "";
}

void SWORD::Material::
set_diffuse(const glm::vec3& diff) {
	info_.diffuse = diff;
}

void SWORD::Material::set_specular(const glm::vec3& spec) {
	info_.specular = spec;
}

void SWORD::Material::set_ambient(const glm::vec3& ambient) {
	info_.ambient = ambient;
}

void SWORD::Material::set_transparent(const glm::vec3& transparent) {
	info_.transparent = transparent;
}

void SWORD::Material::set_wireframe(bool used) {
	info_.wireframe = used;
}

void SWORD::Material::set_twosided(bool b) {
	info_.twosided = b;
}

void SWORD::Material::set_texture(std::string& id, uint8_t idx) {

}
