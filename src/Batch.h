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

class SWORD_EXPORT Batch {
  private:
    size_t num_of_max_vertices_;
    size_t num_of_used_vertices_;
    uint32_t vao_;
    uint32_t vbo_;
    uint32_t mode_;
    uint32_t draw_type_;
    uint32_t shader_;

    glm::vec3 last_;

  public:
    Batch(uint32_t mode,
          uint32_t draw_type,
          uint32_t shader = 0,
          size_t num_of_max_vertices = 1024);

    void init(uint32_t mode,
              uint32_t draw_type,
              uint32_t shader = 0,
              size_t num_of_max_vertices = 1024);

    void resetUsedVertices();

    inline bool empty() {
        return num_of_used_vertices_ == 0;
    }

    void clean();
    //void setVertexAttribPoint(int attribute = 0, int size = 3);
    void render()const;

    bool add(std::vector<glm::vec3>& pos);

};

SWORD_END

#endif




















