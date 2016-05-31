#include "ResourceGroup.h"
#include "LogManager.h"
#include "ResourceDef.h"
#include "Skeleton.h"
#include "Texture.h"
#include "AssimpCodec.h"

SWORD_BEGIN

ResourceGroup::ResourceType
ResourceGroup::Registry::exist(const std::string & id) {
	auto iter = std::get<TYPEINDEX>(resource_map_).find(id);
	if (iter !=std::get<TYPEINDEX>(resource_map_).end()) {
		return iter->second;
	}
	return UNKNOWN;
}

ResourceGroup::ResourceGroup() {
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
}

std::string ResourceGroup::loadModule(const std::string & filename) {

	WIND_LOG_TRACE(DEFAULT_WIND_LOGGER,
				   "start load module:" +
				   filename);

	if (R.exist(generate_id(filename, "module", 0)) != UNKNOWN) {
		WIND_LOG_WARN(DEFAULT_WIND_LOGGER,
					  filename +
					  "has load before");
		return "";
	}
	

	std::vector<Mesh> mesh;
	std::vector<Material> material;
	Skeleton skeleton;
	
	AssimpCodec codec;
	codec.decode(filename);
	codec.load(mesh, material, skeleton);

	ModulePtr p_module(new Module ,[](Module* m) {
		WIND_LOG_DEBUG(DEFAULT_WIND_LOGGER,
					   m->self_id+" has deleted");
		delete m;
	});

	p_module->self_id = generate_id(filename, "module", 0);
	
	SkeletonPtr p_ske(new Skeleton ,[](Skeleton* m) {
		WIND_LOG_DEBUG(DEFAULT_WIND_LOGGER,
					   m->self_id() + "  has deleted");
		delete m;
	});

	*p_ske = skeleton;
	p_ske->set_self_id(generate_id(filename, "skeleton", 0));
	R.signUp<SKELETON>(p_ske->self_id(), p_ske);

	for (size_t i = 0; i < mesh.size(); i++) {
		MeshPtr pm(new Mesh, [](Mesh* m) {
			WIND_LOG_DEBUG(DEFAULT_WIND_LOGGER,
						   m->self_id + "has deleted");
			delete m; 
		});
		
		pm->vertex = std::move(mesh[i].vertex);
		pm->index = std::move(mesh[i].index);
		pm->self_id = generate_id(filename, "mesh", i);
		pm->skeleton_id = p_ske->self_id();
		pm->joint_id = std::move(mesh[i].joint_id);
		pm->joint_weight = std::move(mesh[i].joint_weight);
		pm->material_id = generate_id(filename, "material", mesh[i].material_id);
		pm->texCoord = std::move(mesh[i].texCoord);
		pm->normal = std::move(mesh[i].normal);
		p_module->sub_meshs.push_back(pm->self_id);
		R.signUp<MESH>(pm->self_id, pm);
	}
	
	for (size_t i = 0; i < material.size();++i)
	{
		TexturePtr p_tex(new Texture,[](Texture*m){
			WIND_LOG_DEBUG(DEFAULT_WIND_LOGGER,
						   m->self_id() + " has deleted");
			delete m; });

		MaterialPtr p_mat(new Material, [](Material*m) {
			WIND_LOG_DEBUG(DEFAULT_WIND_LOGGER,
						   m->self_id + " has deleted");
			delete m; });

		std::string path = material[i].tex_diff;
		*p_mat = material[i];
		p_mat->self_id = generate_id(filename, "material", i);
		p_mat->tex_diff = "";
		
		std::string texture_filename = path.substr(path.find_last_of("\\/") + 1);
		if (p_tex->load(texture_filename,false,false)) {
			p_tex->set_self_id(generate_id(filename, "texture", texture_filename));
			R.signUp<TEXTURE>(p_tex->self_id(), p_tex);
			p_mat->tex_diff = p_tex->self_id();
		}

		R.signUp<MATERIAL>(p_mat->self_id, p_mat);
	}

	R.signUp<MODULE>(p_module->self_id, p_module);
	return p_module->self_id;
}

SWORD_END
