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

#include <unordered_map>
#include <string>
#include <tuple>
#include <cassert>
#include <memory>
#include "Platform.h"

SWORD_BEGIN

struct Mesh;
struct Material;
struct Module;
class Texture;
class Skeleton;

typedef  std::shared_ptr<Mesh> MeshPtr;
typedef  std::shared_ptr<Material> MaterialPtr;
typedef  std::shared_ptr<Module> ModulePtr;
typedef  std::shared_ptr<Texture> TexturePtr;
typedef  std::shared_ptr<Skeleton> SkeletonPtr;

class SWORD_EXPORT ResourceGroup {
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

	class Registry {
		friend ResourceGroup;

		enum SpecialKey {
			TYPEINDEX = 5
		};

		template<ResourceType type, class T>
		void signUp(const std::string& id, T resource_ptr) {
			assert(type != UNKNOWN);

			static_assert(std::is_same<T, 
						  std::remove_reference<decltype(std::get<type>(resource_map_))>::
						  type::value_type::second_type>::value
						, "The Resource ID isn't right for the resource type");

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
			<ResourceType type,
			class T = std::remove_reference<decltype(std::get<type>(resource_map_))>::type::value_type::second_type
			>
			T get(const std::string& id) {
			assert(type != UNKNOWN);
			assert(exist(id) != UNKNOWN);
			assert(exist(id) == type);
			return std::get<type>(resource_map_).find(id)->second;
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

	bool loadTexture(const std::string& filename);

	std::string loadModule(const std::string& filename);

	template<ResourceType type>
	auto get(const std::string& id) ->decltype(R.get<type>(id)) {
		return R.get<type>(id);
	}

private:

	Registry R;

};

SWORD_END

#endif // SWORD_RESOURCE_GROUP_H__
