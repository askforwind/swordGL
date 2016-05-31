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

#include "TypeDef.h"
#include "Platform.h"
#include "singleton.h"
#include <unordered_map>
#include <string>
#include <tuple>
#include <cassert>

SWORD_BEGIN

void moveTo(Mesh& from, Mesh& to);
void moveTo(Material& from, Material& to);
void moveTo(Module& from, Module& to);

MeshPtr getMeshPtr(const std::string& mesh_id);

std::string getMaterialId(const std::string& mesh_id);

std::string getSkeletonId(const std::string& mesh_id);

MaterialPtr getMaterialPtr(const std::string& material_id);

SkeletonPtr getSkeletonPtr(MeshPtr mesh);

TexturePtr getDiffuseMapPtr(const MaterialPtr material);

class SWORD_EXPORT ResourceGroup:public Singleton<ResourceGroup> {
public:
	ResourceGroup();
	~ResourceGroup() = default;

	enum ResourceType {
		TEXTURE = 0,
		MESH = 1,
		MATERIAL = 2,
		MODULE = 3,
		SKELETON = 4,
		UNKNOWN = 5,
	};

	TexturePtr loadTexture(const std::string& filename,
						   const std::string& id);

	ModulePtr loadModule(const std::string& filename);

	template<ResourceType type>
	auto get(const std::string& id) ->decltype(R.get<type>(id)) {
		return R.get<type>(id);
	}

private:

	class Registry {
		friend ResourceGroup;

		enum SpecialKey {
			TYPEINDEX = 5
		};

		template<ResourceType type>
		using _RegistryDataType = typename std::remove_reference_t<decltype(std::get<type>(resource_map_))>;

		template<ResourceType type>
		struct RegistryDataType {
			typedef typename _RegistryDataType<type>::value_type::first_type  key;
			typedef typename _RegistryDataType<type>::value_type::second_type value;
		};

		template
			<
			ResourceType type
			,typename value_type = RegistryDataType<type>::value
			>
		void signUp(const std::string& id, value_type resource_ptr) {
			assert(exist(id) == UNKNOWN && "Resource has signed");
			static_assert(type != UNKNOWN, "Resource Type Can't be UNKNOWN");
			std::get<TYPEINDEX>(this->resource_map_).insert(std::make_pair(id, type));
			std::get<type>(this->resource_map_).insert(std::make_pair(id, resource_ptr));
		}

		ResourceType exist(const std::string& id);

		template<ResourceType type>
		void release(const std::string& id) {
			assert(type != UNKNOWN);
			if (exist(id) == UNKNOWN) return;
			assert(exist(id) == type);

			auto iter = std::get<type>(resource_map_).find(id);
			std::get<type>(resource_map_).erase(iter);
		}

		template
			<
			ResourceType type,
			typename value_type = RegistryDataType<type>::value
			>
			value_type get(const std::string& id) {
			assert(type != UNKNOWN);
			assert(exist(id) != UNKNOWN);
			assert(exist(id) == type);
			auto iter = std::get<type>(resource_map_).find(id);
			return iter == std::get<type>(resource_map_).end() ?
				nullptr : iter->second;
		}

		std::tuple
			<
			std::unordered_map<std::string, std::shared_ptr<Texture> >
			, std::unordered_map<std::string, std::shared_ptr<Mesh> >
			, std::unordered_map<std::string, std::shared_ptr<Material> >
			, std::unordered_map<std::string, std::shared_ptr<Module> >
			, std::unordered_map<std::string, std::shared_ptr<Skeleton> >
			, std::unordered_map<std::string, ResourceType>
			> resource_map_;

	};

	

	Registry R;

};

SWORD_END

#endif // SWORD_RESOURCE_GROUP_H__
