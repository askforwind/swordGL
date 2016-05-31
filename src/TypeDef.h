#ifndef SWORD_TYPEDEF_H__
#define SWORD_TYPEDEF_H__

#include <memory>

namespace SWORD {
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
	typedef  std::weak_ptr<Mesh> MeshWeakPtr;
	typedef  std::weak_ptr<Material> MaterialWeakPtr;
	typedef  std::weak_ptr<Module> ModuleWeakPtr;
	typedef  std::weak_ptr<Texture> TextureWeakPtr;
	typedef  std::weak_ptr<Skeleton> SkeletonWeakPtr;

	class Technique;
	typedef std::shared_ptr<Technique> TechniquePtr;
	typedef std::weak_ptr<Technique> TechniqueWeakPtr;

	class Entity;
	typedef std::shared_ptr<Entity> EntityPtr;
	typedef std::weak_ptr<Entity> EntityWeakPtr;
}

#endif // SWORD_TYPEDEF_H__
