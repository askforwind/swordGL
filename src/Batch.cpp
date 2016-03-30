#include"Batch.h"
#include"LogManager.h"
#include<GL/glew.h>
#include<GL/glext.h>
#include<GL/glu.h>
#include<cassert>
#include<iostream>

SWORD_BEGIN

Batch::Batch(uint32_t mode,
             uint32_t draw_type,
             uint32_t shader,
             size_t num_of_max_vertices)

    : num_of_max_vertices_(0),
      num_of_used_vertices_(0),
      vao_(0),
      vbo_(0),
      mode_(GL_POINTS),
      draw_type_(0),
      shader_(0),
      last_(glm::vec3()) {
    init(mode, draw_type, shader, num_of_max_vertices);
}

void Batch::resetUsedVertices() {
    num_of_used_vertices_ = 0;
    last_ = glm::vec3();
}

void Batch::init(uint32_t mode,
                 uint32_t draw_type,
                 uint32_t shader ,
                 size_t num_of_max_vertices) {

    clean();

    draw_type_ = draw_type;
    num_of_max_vertices_ = num_of_max_vertices;
    mode_ = mode;
    shader_ = shader;

    assert(num_of_max_vertices_ >= 1024 &&
           "batch size is smaller than 1024");


    glGenVertexArrays(1, &vao_);

    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*num_of_max_vertices_, NULL, draw_type_);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    GLenum err = glGetError();
    if(err != GL_NO_ERROR) {
        WIND_LOG_ERROR(WIND::LogManager::get_default_console_logger(),
                       gluErrorString(err));
        exit(0);
    }
}

void Batch::clean() {
    if(vbo_) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }
    if(vao_) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }
    if(shader_) {
        shader_ = 0;
    }
    num_of_used_vertices_ = 0;
    last_ = glm::vec3();
}

void Batch::render() const {
    if(!vao_ || !vbo_) return;

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    glUseProgram(shader_);
    glDrawArrays(mode_, 0, static_cast<int>(num_of_used_vertices_));
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool Batch::add(std::vector<glm::vec3>& vec) {

    if(vec.empty()) {
        WIND_LOG_WARN(WIND::LogManager::get_default_console_logger(),
                      "empty vertes!");
        return true;
    }

    size_t num_extra_ver = mode_ == GL_TRIANGLE_STRIP ? 2 : 0;

    if(num_of_max_vertices_ <
            num_of_used_vertices_ + num_extra_ver + vec.size()) {
        WIND_LOG_WARN(WIND::LogManager::get_default_console_logger(),
                      "batch has not enough space for vertes");
        return false;
    }

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    if(num_extra_ver > 0) {
        glBufferSubData(GL_ARRAY_BUFFER,
                        num_of_used_vertices_ * sizeof(glm::vec3),
                        sizeof(glm::vec3),
                        &last_);
        glBufferSubData(GL_ARRAY_BUFFER,
                        (num_of_used_vertices_ + 1)*sizeof(glm::vec3),
                        sizeof(glm::vec3),
                        vec.data());
    }

    glBufferSubData(GL_ARRAY_BUFFER,
                    (num_of_used_vertices_ + num_extra_ver)*sizeof(glm::vec3),
                    sizeof(glm::vec3)*vec.size(),
                    vec.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    num_of_used_vertices_ += num_extra_ver + vec.size();
    last_ = vec.back();

    return true;
}

SWORD_END

























































