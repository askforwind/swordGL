/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-03-28 14:47
#
# Filename: Batch.h
#
# Description:
#
==================================================================*/
#ifndef SWORD_BATCH_H_
#define SWORD_BATCH_H_

#include"Platform.h"
#include<stdint.h>
#include<vector>
#include<glm/glm.hpp>

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


class SWORD_EXPORT Batch {
  private:
    size_t num_of_max_vertices_;
    size_t num_of_used_vertices_;
    uint32_t vao_;
    uint32_t vbo_;

    glm::vec3 last_;
    BatchConfig config_;


  public:
    Batch(uint32_t mode,
          uint32_t draw_type,
          uint32_t shader = 0,
          size_t vertices_limit = 1024);

    Batch(BatchConfig& config, size_t vertices_limit = 1024);

    void init(uint32_t mode,
              uint32_t draw_type,
              uint32_t shader = 0,
              size_t vertices_limit = 1024);

    void init (BatchConfig& config, size_t vertices_limit = 1024);

    inline bool empty() {
        return num_of_used_vertices_ == 0;
    }

    inline bool uninited() {
        return num_of_max_vertices_ == 0;
    }

    void deinit();
    //void setVertexAttribPoint(int attribute = 0, int size = 3);
    void render()const;

    bool load(std::vector<glm::vec3>& pos);

    void unload();

};

SWORD_END

#endif
































