#include"BatchManager.h"
#include<stdlib.h>

SWORD_BEGIN
void BatchManager::addToStaticBatch(std::vector<glm::vec3>& vec,
                                    uint32_t mode,
                                    uint32_t draw_type,
                                    uint32_t shader) {
    BatchConfig config = {mode, draw_type, shader};

    auto i = static_batch_list_.lower_bound(config);
    auto end = static_batch_list_.upper_bound(config);

    for(; i != end; ++i) {
        if(i->second.load(vec) == true) {
            return;
        }
    }

    Batch ba = Batch(config, vec.size() + 1024);
    ba.load(vec);
    static_batch_list_.insert(std::make_pair( config, ba));
}

void BatchManager::addToDynamicBatch(std::vector<glm::vec3>& vec,
                                     uint32_t mode,
                                     uint32_t draw_type,
                                     uint32_t shader) {
    BatchConfig config = {mode, draw_type, shader};

    auto i = dynamic_batch_list_.lower_bound(config);
    auto end = dynamic_batch_list_.upper_bound(config);

    for(; i != end; ++i) {
        if(i->second.load(vec) == true) {
            return;
        }
    }

    Batch ba = Batch(config, vec.size() + 1024);
    ba.load(vec);
    dynamic_batch_list_.insert(std::make_pair( config, ba));

}

void BatchManager::removeEmptyDynamicBatch() {
    for(auto i = dynamic_batch_list_.begin(); i != dynamic_batch_list_.end();) {
        if(i->second.empty()) {
            i++;
            dynamic_batch_list_.erase(std::prev(i, 1));
        } else {
            i++;
        }
    }
}

void BatchManager::drawAllBatch() {

    for(std::pair<const BatchConfig, Batch>& i : static_batch_list_)
        i.second.render();
    for(std::pair<const BatchConfig, Batch>& i : dynamic_batch_list_)
        i.second.render();
}

void BatchManager::destroyAllBatch() {
    static_batch_list_.clear();
    dynamic_batch_list_.clear();
}


void BatchManager::cleanStaticBatch() {
    for(std::pair<const BatchConfig, Batch>& i : static_batch_list_)
        i.second.unload();
}

void BatchManager::cleanDynamicBatch() {
    for(std::pair<const BatchConfig, Batch>& i : dynamic_batch_list_)
        i.second.unload();
}
SWORD_END



































































