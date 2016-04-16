/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: ResourceGroup.h
#
# Last modified:2016-4-15
#
# Description:
#
#=====================================*/

#ifndef SWORD_RESOURCE_GROUP_H__
#define SWORD_RESOURCE_GROUP_H__

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include <unordered_map>
#include <string>

struct aiMesh;
struct aiMaterial;
SWORD_BEGIN

typedef  std::vector<std::string> Module;

class SWORD_EXPORT ResourceGroup {
public:
	ResourceGroup();
	~ResourceGroup();

	static void initialise();

	static void deinitialise();

	Texture* loadTexture(const std::string& path,const std::string& id);

	Module* loadModule(const std::string& path,const std::string& id);

	const Texture* get_texture(const std::string& id);

	const Mesh* get_mesh(const std::string& id);

	const Material* get_material(const std::string& id);

private:

	void loadMesh(const std::string& id,const aiMesh* m,const std::string& mat_id);

	void loadMaterial(const std::string& id, const aiMaterial* m);

	std::unordered_map<std::string, Texture> all_texture_;
	std::unordered_map<std::string, Mesh> all_mesh_;
	std::unordered_map<std::string, Material> all_material_;
	std::unordered_map<std::string, Module> all_module_;
};


SWORD_END

#endif // SWORD_RESOURCE_GROUP_H__
