#include"Application.h"
#include<GL/gl.h>
#include"shader.hpp"
#include<iostream>
#include<glm/glm.hpp>
Application::Application()
    : quit_(false),
      use_msaa_(true) ,
      evt_() {
    root_ = new SWORD::Root;
}

Application::~Application() {
    delete root_;
}

void Application::update(float d) {

    SDL_PollEvent(&evt_);
    switch(evt_.type) {
    case SDL_QUIT:
        quit_ = true;
        break;
    case SDL_KEYDOWN:
        keyPressed(evt_.key);
        break;
    case SDL_MOUSEBUTTONDOWN:
        mousePressed(evt_.button);
        break;
    }

    if(!bezier_curve_.empty()) {
        root_->get_batch_manager()->addToDynamicBatch(bezier_curve_, GL_LINE_STRIP, GL_STREAM_DRAW, 0/* proId*/);
    }
}
void Application::run() {

    WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                   "start render");

    glEnable(GL_MULTISAMPLE);
    glPointSize(10);
    glLineWidth(1);
    createSince();

    timer_.begin();
    while(!quit_) {

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        timer_.tick();
        update(timer_.sinceLastTick());
        render(timer_.sinceLastTick());
        showFPS(timer_.sinceLastTick());
        //batch_mgr_.cleanDynamicBatch();
        root_->get_batch_manager()->resetDynamicBatchVertex();
        root_->swapBuffer();
    }
    destroySince();
}

void Application::showFPS(float diff) {
    static int fps = 0;
    static float time_since_last_second = 0.0f;
    fps++;
    time_since_last_second += diff;

    if(time_since_last_second >= 1000.0f) {
        root_->get_defaule_render_window()->set_title(std::to_string(fps).c_str());
        time_since_last_second = 0.0f;
        fps = 0;
    }
}

void Application::createSince() {
    WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                   "start create since");
    uint32_t proId = LoadShaders( "SimpleVertexShader.glsl", "SimpleFragmentShader.glsl" );
    //uint32_t matId = glGetUniformLocation(proId, "MVP");
    //glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    std::vector<glm::vec3> vec = {glm::vec3(-0.5, -0.5, 0.0),
                                  glm::vec3(0.0, 0.5, 0.0),
                                  glm::vec3(0.5, -0.5, 0.0)
                                 };
    root_->get_batch_manager()->addToStaticBatch(vec, GL_POINTS, GL_STATIC_DRAW, proId);
}

void Application::destroySince() {
    WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                   "remove " +
                   std::to_string(root_->get_batch_manager()->get_num_of_batch()) +
                   " batchs in Application");

    root_->get_batch_manager()->destroyAllBatch();
    std::cout << bezier_curve_.size() << std::endl;
}

void Application::render(float diff) {
    (void)(diff);
    root_->get_batch_manager()->drawAllBatch();
}




































































