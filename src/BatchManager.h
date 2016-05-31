/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-04-03 16:56
#
# Filename: BatchManager.h
#
# Description:
#
==================================================================*/

#ifndef SWORD_BATCHMANAGER_H_
#define SWORD_BATCHMANAGER_H_
#include"Batch.h"
#include<map>

SWORD_BEGIN


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

    // destroy all batch，clear static_list and dynamic_list,
    // all vertex and config infomation will be lost,
    // nothing will be drawing if you don't load new vertex
    void destroyAllBatch();

    // unload all vertexs in static batch ,but
    // the member static_batch_list_ won't be clear,
    // just set the num of used vertex to zero;
    void cleanStaticBatch();

    // unload all vertexs in dynamic batch ,but
    // the member dynamic_batch_list_ won't be clear,
    // just set the num of used vertex to zero;
    // thid method will be call when Root swapBuffer(every frame)
    void cleanDynamicBatch();

    // the dynamic_batch_list_ will erase the batch which is empty
    void removeEmptyDynamicBatch();

    inline void cleanAllBatch() {
        cleanStaticBatch(), cleanDynamicBatch();
    }

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


#endif














































