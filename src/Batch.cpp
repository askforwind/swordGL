#include"Batch.h"
#include"LogManager.h"
#include<GL/glew.h>
#include<GL/glext.h>
#include<cassert>

SWORD_BEGIN

Batch::Batch(uint32_t mode,
             uint32_t draw_type,
             uint32_t shader,
             size_t vertices_limit)

    : num_of_max_vertices_(0),
      num_of_used_vertices_(0),
      vao_(0),
      vbo_(0),
      last_(glm::vec3()),
    config_({GL_TRIANGLE_STRIP, GL_STATIC_DRAW, 0}) {
    init(mode, draw_type, shader, vertices_limit);
}

Batch::Batch(BatchConfig& config, size_t vertices_limit)

    : num_of_max_vertices_(0),
      num_of_used_vertices_(0),
      vao_(0),
      vbo_(0),
      last_(glm::vec3()),
    config_({GL_TRIANGLE_STRIP, GL_STATIC_DRAW, 0}) {
    init(config, vertices_limit);
}

Batch::~Batch() {
    deinit();
}

void Batch::unload() {
    num_of_used_vertices_ = 0;
    last_ = glm::vec3();
}

void Batch::init(BatchConfig& config, size_t vertices_limit) {
    init(config.mode, config.draw_type, config.shader, vertices_limit);
}

void Batch::init(uint32_t mode,
                 uint32_t draw_type,
                 uint32_t shader ,
                 size_t vertices_limit) {

    deinit();

    config_ = {mode, draw_type, shader};
    num_of_max_vertices_ = vertices_limit;

    assert(num_of_max_vertices_ >= 1024 &&
           "batch size is smaller than 1024");


    glGenVertexArrays(1, &vao_);

    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*num_of_max_vertices_, NULL, config_.draw_type);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    GLenum err = glGetError();
    if(err != GL_NO_ERROR) {
        WIND_LOG_ERROR(DEFAULT_WIND_LOGGER, gluErrorString(err));
        exit(0);
    }
}

void Batch::deinit() {
    if(vbo_) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }
    if(vao_) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }

    config_.shader = 0;

    num_of_used_vertices_ = 0;
    num_of_max_vertices_ = 0;
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
    glUseProgram(config_.shader);
    glDrawArrays(config_.mode, 0, 1)/*static_cast<int>(num_of_used_vertices_))*/;
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool Batch::load(std::vector<glm::vec3>& vec) {

    if(vec.empty()) {
        WIND_LOG_WARN(DEFAULT_WIND_LOGGER,
                      "empty vertes!");
        return true;
    }

    size_t num_extra_ver = config_.mode == GL_TRIANGLE_STRIP ? 2 : 0;

    if(num_of_max_vertices_ <
            num_of_used_vertices_ + num_extra_ver + vec.size()) {
        WIND_LOG_WARN(DEFAULT_WIND_LOGGER,
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
    GLenum err = glGetError();
    if(err != GL_NO_ERROR) {
        WIND_LOG_ERROR(DEFAULT_WIND_LOGGER, gluErrorString(err));
        exit(0);
    }

    return true;
}

SWORD_END
































































































