#include <algorithm>
#include "ResourceGroup.h"
#include "LogManager.h"
#include "FreeImage.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "assimp\scene.h"
SWORD_BEGIN

void ResourceGroup::initialise() {
	FreeImage_Initialise();
	FreeImage_SetOutputMessage(
		[](FREE_IMAGE_FORMAT fif, const char* message)->void {
		std::string old_console_format = WIND::LogManager::
			set_default_console_logger_format("%D{%H:%M:%S} %p: %m\n");
		std::string old_file_format = WIND::LogManager::
			set_default_file_logger_format("%D{%H:%M:%S} %p: %m\n");

		std::string error = "Image Error:";
		WIND_LOG_ERROR(DEFAULT_WIND_LOGGER,
					   error
					   + FreeImage_GetFormatFromFIF(fif)
					   + " " + message);

		WIND::LogManager::set_default_console_logger_format(old_console_format);
		WIND::LogManager::set_default_file_logger_format(old_file_format);
	});
}

void ResourceGroup::deinitialise() {
	FreeImage_DeInitialise();
}

ResourceGroup::ResourceGroup()
	:all_texture_()
	, all_mesh_()
	, all_material_()
	, all_module_() {
}

ResourceGroup::~ResourceGroup() {
	auto iter = all_texture_.begin();
	for (; iter != all_texture_.end(); iter++) {
		iter->second.unload();
	}

	all_texture_.clear();
	all_material_.clear();
	all_mesh_.clear();
	all_module_.clear();
}


Texture* ResourceGroup::loadTexture(const std::string& path,
								const std::string& id) {

	WIND_LOG_TRACE(DEFAULT_WIND_LOGGER,
				   "start load texture:" + path +
				   "  id:" + id);

	if (all_texture_.find(id) != all_texture_.end()) {
		WIND_LOG_ERROR(DEFAULT_WIND_LOGGER,
					   "Could't load texture:" + path +
					   " because the id:" + id + "was occpied");
		return nullptr;
	}

	Texture t;
	if (!t.load(path.c_str())) {
		WIND_LOG_ERROR(DEFAULT_WIND_LOGGER,
					   "Could't load texture:" + path +
					   " id:" + id);
		return nullptr;
	}

	auto r = all_texture_.insert(std::make_pair(id, t));
	return &r.first->second;
}

Module* ResourceGroup::loadModule(const std::string& path,
							   const std::string& id) {
	WIND_LOG_TRACE(DEFAULT_WIND_LOGGER,
				   "start load module:" + path +
				   "  id:" + id);

	if (all_module_.find(id) != all_module_.end()) {
		WIND_LOG_ERROR(DEFAULT_WIND_LOGGER,
					   "Could't load module:" + path +
					   "because the id:" +
					   id + "was occpied");
		return nullptr;
	}

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path,
										   aiProcessPreset_TargetRealtime_Quality);
	if (!scene) {
		WIND_LOG_ERROR(DEFAULT_WIND_LOGGER,
					   "Could't load module:" + path +
					   " id:" + id +
					   " error:" + import.GetErrorString());
		return nullptr;
	}

	Module module(scene->mNumMeshes, "");

	std::string mat_id,mesh_id;
	for (uint32_t i=0;i<scene->mNumMeshes;++i){
		const aiMesh* m = scene->mMeshes[i];
		mat_id = id + "_material_" + std::to_string(i);
		mesh_id = id + "_mesh_" + std::to_string(i);
		loadMesh(mesh_id, m, mat_id);
		loadMaterial(mat_id, scene->mMaterials[m->mMaterialIndex]);
		module[i] = mesh_id;
	}
	
	auto r = all_module_.insert(std::make_pair(id, module));
	return &r.first->second;
}

void ResourceGroup::loadMaterial(const std::string& id, 
								 const aiMaterial* m) {
	
	if (all_material_.find(id) != all_material_.end()) {
		WIND_LOG_ERROR(DEFAULT_WIND_LOGGER,
					   "Could't load material,because the id:" +
					   id + "was occpied");
		return;
	}

	auto r = all_material_.insert(std::make_pair(id, Material()));
	Material& material = r.first->second;
	
	aiColor3D c;
	if (m->Get(AI_MATKEY_COLOR_DIFFUSE, c) == AI_SUCCESS) {
		material.set_diffuse(glm::vec3(c.r, c.g, c.b));
	}
	if (m->Get(AI_MATKEY_COLOR_AMBIENT, c) == AI_SUCCESS) {
		material.set_ambient(glm::vec3(c.r, c.g, c.b));
	}
	if (m->Get(AI_MATKEY_COLOR_SPECULAR, c) == AI_SUCCESS) {
		material.set_specular(glm::vec3(c.r, c.g, c.b));
	}
	//TODO
	
}

void ResourceGroup::loadMesh(const std::string& id,
							 const aiMesh* m,
							 const std::string& mat_id) {
	if (all_mesh_.find(id) != all_mesh_.end()) {
		WIND_LOG_ERROR(DEFAULT_WIND_LOGGER,
					   "Could't load mesh,because the id:" +
					   id + "was occpied");
		return;
	}

	auto r = all_mesh_.insert(std::make_pair(id, Mesh()));
	Mesh& mesh = r.first->second;
	mesh.set_material(mat_id);
	//------read vertex-------
	std::vector<glm::vec3> vertex(m->mNumVertices, glm::vec3(0.0f));
	for (uint32_t i = 0; i < m->mNumVertices; ++i) {
		aiVector3D* vec = m->mVertices + i;
		vertex[i] = glm::vec3(vec->x, vec->y, vec->z);
	}

	mesh.readVertex(std::move(vertex));

	//-------read index--------;
	std::vector<uint16_t> idx(m->mNumFaces * 3, 0);
	for (uint32_t i = 0; i < m->mNumFaces; ++i) {
		aiFace& f = m->mFaces[i];
		idx[3 * i] = f.mIndices[0];
		idx[3 * i + 1] = f.mIndices[1];
		idx[3 * i + 2] = f.mIndices[2];
		assert(f.mNumIndices == 3);
	}

	mesh.readIndex(std::move(idx));

	//-------read color----------;
	std::vector<glm::vec4> color(m->mNumVertices, glm::vec4(1.0f));
	
	if (m->HasVertexColors(0)) {
		for (uint32_t i = 0; i < m->mNumVertices; ++i) {
			aiColor4D& c = m->mColors[0][i];
			color[i] = glm::vec4(c.r, c.b, c.g, c.a);
		}
	}
	mesh.readColor(std::move(color));

	//-------read uv----------;
	std::vector<glm::vec2> uv(m->mNumVertices,glm::vec2(1.0f));
	if (m->HasTextureCoords(0)) {
		for (uint32_t i = 0; i < m->mNumVertices; ++i) {
			aiVector3D& tc = m->mTextureCoords[0][i];
			uv[i] = glm::vec2(tc.x, tc.y);
		}
	}
	mesh.readUV(std::move(uv));

	//-------read normal----------;
	std::vector<glm::vec3> normal(m->mNumFaces, glm::vec3(0.0f));
	if (m->HasNormals()) {
		for (uint32_t i = 0; i < m->mNumFaces; ++i) {
			aiVector3D& n = m->mNormals[i];
			normal[i] = glm::vec3(n.x, n.y, n.z);
		}
	}
	mesh.readNormal(std::move(normal));
}

const Material* ResourceGroup::get_material(const std::string& id) {
	auto iter = all_material_.find(id);
	return iter == all_material_.end() ? NULL : &iter->second;
}

const Mesh* ResourceGroup::get_mesh(const std::string& id) {
	auto iter = all_mesh_.find(id);
	return iter == all_mesh_.end() ? NULL : &iter->second;
}


const Texture* ResourceGroup::get_texture(const std::string& id) {
	auto iter = all_texture_.find(id);
	return iter == all_texture_.end() ? NULL : &iter->second;
}

SWORD_END