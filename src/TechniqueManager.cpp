#include "TechniqueManager.h"
#include "Technique.h"

SWORD_BEGIN

TechniqueManager* Singleton<TechniqueManager>::msSingleton = nullptr;

TechniquePtr TechniqueManager::creatTechnique(const std::string& id) {
	assert(!findTechnique(id));
	TechniquePtr tech = std::make_shared<Technique>();
	technique_list_.insert(std::make_pair(id, tech));
	tech->init();
	tech->set_self_id(id);
	return tech;
}

TechniquePtr TechniqueManager::get(const std::string& id) {
	auto iter = technique_list_.find(id);
	return iter == technique_list_.end() ? nullptr : iter->second;
}

bool TechniqueManager::findTechnique(const std::string& key) {
	return technique_list_.find(key) != technique_list_.end();
}

SWORD_END