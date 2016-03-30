#include"BatchManager.h"
#include<stdlib.h>

SWORD_BEGIN
void BatchManager::addToStaticBatch(std::vector<glm::vec3>& vec,
                                    uint32_t mode,
                                    uint32_t draw_type,
                                    uint32_t shader) {
    BatchConfig config;
    config.mode = mode;
    config.draw_type = draw_type;
    config.shader = shader;

    bool ok = false;

    auto i = static_batch_list_.lower_bound(config);
    auto end = static_batch_list_.upper_bound(config);

    for(; i != end; ++i) {
        if(i->second.add(vec) == true) {
            ok = true;
            break;
        }
    }

    if(!ok) {
        Batch ba = Batch(mode, draw_type, shader, vec.size() + 1024);
        ba.add(vec);
        static_batch_list_.insert(std::make_pair( config, ba));
    }

}

void BatchManager::addToDynamicBatch(std::vector<glm::vec3>& vec,
                                     uint32_t mode,
                                     uint32_t draw_type,
                                     uint32_t shader) {
    BatchConfig config;
    config.mode = mode;
    config.draw_type = draw_type;
    config.shader = shader;

    bool ok = false;

    auto i = dynamic_batch_list_.lower_bound(config);
    auto end = dynamic_batch_list_.upper_bound(config);

    for(; i != end; ++i) {
        if(i->second.add(vec) == true) {
            ok = true;
            break;
        }
    }

    if(!ok) {
        Batch ba = Batch(mode, draw_type, shader, vec.size() + 10240);
        ba.add(vec);
        dynamic_batch_list_.insert(std::make_pair( config, ba));
    }

}

void BatchManager::removeExtraEmptyDynamicBatch() {
    for(auto i = dynamic_batch_list_.begin();
            i != dynamic_batch_list_.end();) {

        if(i->second.empty()) {
            auto temp = i;
            i++;
            dynamic_batch_list_.erase(temp);
        } else {
            i++;
        }
    }
}

void BatchManager::resetDynamicBatchVertex() {
    for(std::pair<const BatchConfig, Batch>& i : dynamic_batch_list_)
        i.second.resetUsedVertices();
}

void BatchManager::drawAllBatch() {

    for(std::pair<const BatchConfig, Batch>& i : static_batch_list_)
        i.second.render();
    for(std::pair<const BatchConfig, Batch>& i : dynamic_batch_list_) {
        i.second.render();
    }
}

void BatchManager::cleanAllBatch() {
    cleanStaticBatch();
    cleanDynamicBatch();
}

void BatchManager::destroyAllBatch() {
    cleanAllBatch();
    static_batch_list_.clear();
    dynamic_batch_list_.clear();
}

void BatchManager::cleanStaticBatch() {
    for(std::pair<const BatchConfig, Batch>& i : static_batch_list_)
        i.second.clean();
}

void BatchManager::cleanDynamicBatch() {
    for(std::pair<const BatchConfig, Batch>& i : dynamic_batch_list_)
        i.second.clean();
}
SWORD_END












































