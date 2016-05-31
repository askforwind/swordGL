#include "ResourceGroup.h"
#include "LogManager.h"
#include "ResourceDef.h"
#include "Skeleton.h"
#include "Texture.h"
#include "AssimpCodec.h"

SWORD_BEGIN

ResourceGroup* Singleton<ResourceGroup>::msSingleton = nullptr;

void moveTo(Mesh& from, Mesh& to) {
	to.vertex = std::move(from.vertex);
	to.normal = std::move(from.normal);
	to.color = std::move(from.color);
	to.index = std::move(from.index);
	to.texCoord = std::move(from.texCoord);
	to.joint_id = std::move(from.joint_id);
	to.joint_weight = std::move(from.joint_weight);
	to.material_id = std::move(from.material_id);
	to.skeleton_id = std::move(from.skeleton_id);
	to.pre_trans = from.pre_trans;
	to.id = std::move(from.id);
}

void moveTo(Material& from, Material& to) {
	to.diffuse = from.diffuse;
	to.specular = from.specular;
	to.ambient = from.ambient;
	to.emissive = from.emissive;
	to.shininess = from.shininess;
	to.twosided = from.twosided;
	to.transparent = from.transparent;
	
	to.tex_diff = std::move(from.tex_diff);
	to.id = std::move(from.id);
}

void moveTo(Module& from, Module& to) {
	to.sub_meshs = std::move(from.sub_meshs);
	to.id = std::move(from.id);
}

MeshPtr getMeshPtr(const std::string& mesh_id) {
	return ResourceGroup::instance().
		get<ResourceGroup::MESH>(mesh_id);
}

std::string getMaterialId(const std::string& mesh_id) {
	MeshPtr m = ResourceGroup::instance().get<ResourceGroup::MESH>(mesh_id);
	return m->material_id;
}
std::string getSkeletonId(const std::string& mesh_id) {
	MeshPtr m = ResourceGroup::instance().get<ResourceGroup::MESH>(mesh_id);
	return m->skeleton_id;
}

MaterialPtr getMaterialPtr(const std::string& material_id) {
	return ResourceGroup::instance().
		get<ResourceGroup::MATERIAL>(material_id);
}

SkeletonPtr getSkeletonPtr(MeshPtr mesh) {
	return mesh->skeleton_id.empty() ? nullptr : ResourceGroup::instance().
		get<ResourceGroup::SKELETON>(mesh->skeleton_id);
}

TexturePtr getDiffuseMapPtr(const MaterialPtr material) {
	return ResourceGroup::instance().
		get <ResourceGroup::TEXTURE>(material->tex_diff);
}

ResourceGroup::ResourceType
ResourceGroup::Registry::exist(const std::string & id) {
	auto iter = std::get<TYPEINDEX>(resource_map_).find(id);
	if (iter != std::get<TYPEINDEX>(resource_map_).end()) {
		return iter->second;
	}
	return UNKNOWN;
}

ResourceGroup::ResourceGroup()
	:Singleton<ResourceGroup>(){
}

namespace {
	std::string generate_id(const std::string& base,
							const std::string& ext,
							int idx) {
		return base + "_" + ext + "_" + std::to_string(idx);
	}

	std::string generate_id(const std::string& base,
							const std::string& ext,
							const std::string idx) {
		return base + "_" + ext + "_" + idx;
	}

	template<class T>
	std::shared_ptr<T> creatResourcePtr() {
		std::shared_ptr<T> ptr(new T, [](T*m) {
			WIND_LOG_DEBUG(DEFAULT_WIND_LOGGER,
						   m->self_id() + " has deleted");
			delete m;
		});
		return ptr;
	}

}

TexturePtr ResourceGroup::loadTexture(const std::string & filename,
									  const std::string& id) {
	TexturePtr pTex = creatResourcePtr<Texture>();
	pTex->set_self_id(id);
	if (pTex->load(filename)) return pTex;

	return nullptr;
}


ModulePtr ResourceGroup::loadModule(const std::string & filename) {

	WIND_LOG_TRACE(DEFAULT_WIND_LOGGER,
				   "start load module:" +
				   filename);

	if (R.exist(generate_id(filename, "module", 0)) != UNKNOWN) {
		WIND_LOG_WARN(DEFAULT_WIND_LOGGER,
					  filename +
					  "has load before");
		return nullptr;
	}


	std::vector<Mesh> mesh;
	std::vector<Material> material;
	Skeleton skeleton;
	bool has_skeleton;

	AssimpCodec codec;
	codec.decode(filename);
	codec.load(mesh, material, skeleton, has_skeleton);

	ModulePtr p_module = creatResourcePtr<Module>();
	p_module->set_self_id(generate_id(filename, "module", 0));


	SkeletonPtr p_ske = creatResourcePtr<Skeleton>();
	skeleton.moveTo(*p_ske);
	p_ske->set_self_id(generate_id(filename, "skeleton", 0));

	if (has_skeleton)
		R.signUp<SKELETON>(p_ske->self_id(), p_ske);

	int mesh_idx = 0;
	for (size_t i = 0; i < mesh.size(); i++) {
		if (mesh[i].vertex.empty() || mesh[i].index.empty()) continue;

		MeshPtr pm = creatResourcePtr<Mesh>();
		moveTo(mesh[i], *pm);

		pm->set_self_id(generate_id(filename, "mesh", mesh_idx++));
		pm->material_id = generate_id(filename, "material", pm->material_id);
		pm->skeleton_id = has_skeleton ? p_ske->self_id() : "";
		p_module->sub_meshs.push_back(pm->self_id());

		R.signUp<MESH>(pm->self_id(), pm);
	}

	for (size_t i = 0; i < material.size(); ++i) {
		MaterialPtr p_mat = creatResourcePtr<Material>();

		moveTo(material[i], *p_mat);
		p_mat->set_self_id(generate_id(filename, "material", i));
		R.signUp<MATERIAL>(p_mat->self_id(), p_mat);

		if (p_mat->tex_diff.empty()) continue;

		std::string path = p_mat->tex_diff;
		p_mat->tex_diff.clear();

		std::string texture_filename = path.substr(path.find_last_of("\\/") + 1);
		TexturePtr p_tex = loadTexture("D:/swordGL/resource/" + texture_filename,
									   generate_id(filename, "texture", texture_filename));

		if (p_tex != nullptr) {
			R.signUp<TEXTURE>(p_tex->self_id(), p_tex);
			p_mat->tex_diff = p_tex->self_id();
		}
		
	}



	R.signUp<MODULE>(p_module->self_id(), p_module);
	return p_module;
}

SWORD_END
