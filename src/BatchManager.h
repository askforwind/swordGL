#include"Batch.h"
#include<map>

SWORD_BEGIN

struct BatchConfig {
    uint32_t mode;
    uint32_t draw_type;
    uint32_t shader;

    bool operator >(const BatchConfig& rhs)const {
        if(mode > rhs.mode) return true;
        else if(mode == rhs.mode && draw_type > rhs.draw_type) return true;
        else if(mode == rhs.mode && draw_type == rhs.draw_type && shader > rhs.shader) return true;
        return false;
    }
    bool operator <(const BatchConfig& rhs)const {
        return !(*this == rhs || *this > rhs);
    }

    bool operator ==(const BatchConfig& rhs)const {
        return mode == rhs.mode
               && draw_type == rhs.draw_type
               && shader == rhs.shader;
    }

    bool operator !=(const BatchConfig& rhs)const {
        return !(*this == rhs);
    }

};


class SWORD_EXPORT BatchManager {
  public:
    void addToStaticBatch(std::vector<glm::vec3>& vec,
                          uint32_t mode,
                          uint32_t draw_type,
                          uint32_t shader);

    void addToDynamicBatch(std::vector<glm::vec3>& vec,
                           uint32_t mode,
                           uint32_t draw_type,
                           uint32_t shader);

    void drawAllBatch();

    void cleanAllBatch();

    void destroyAllBatch();

    void cleanStaticBatch();

    void cleanDynamicBatch();

    void resetDynamicBatchVertex();

    void removeExtraEmptyDynamicBatch();

    inline size_t get_num_of_batch()const {
        return static_batch_list_.size() + dynamic_batch_list_.size();
    }

    inline size_t get_num_of_dynamic_batch()const {
        return dynamic_batch_list_.size();
    }

  private:
    std::multimap<BatchConfig, Batch> static_batch_list_;
    std::multimap<BatchConfig, Batch> dynamic_batch_list_;
};

SWORD_END






































