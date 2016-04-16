#include "Mesh.h"


SWORD::Mesh::
Mesh() 
	: info_(){
}

void SWORD::Mesh::
readVertex(const std::vector<glm::vec3>& vertex) {
	this->info_.vertex = vertex;
}

void SWORD::Mesh::
readVertex(std::vector<glm::vec3>&& vertex) {
	this->info_.vertex = std::move(vertex);
}


void SWORD::Mesh::
readIndex(const std::vector<uint16_t>& index) {
	this->info_.index = index;
}

void SWORD::Mesh::
readIndex(std::vector<uint16_t>&& index) {
	this->info_.index = std::move(index);
}

void SWORD::Mesh::
readColor(const std::vector<glm::vec4>& color) {
	this->info_.color = color;
}

void SWORD::Mesh::
readColor(std::vector<glm::vec4>&& color) {
	this->info_.color = std::move(color);
}

void SWORD::Mesh::
readUV(const std::vector<glm::vec2>& uv) {
	this->info_.uv = uv;
}

void SWORD::Mesh::readUV(std::vector<glm::vec2>&& uv) {
	this->info_.uv = std::move(uv);
}

void SWORD::Mesh::readNormal(const std::vector<glm::vec3>& normal) {
	this->info_.normal = normal;
}

void SWORD::Mesh::readNormal(std::vector<glm::vec3>&& normal) {
	this->info_.normal = std::move(normal);
}

void SWORD::Mesh::set_material(const std::string& id) {
	info_.material_id = id;
}
