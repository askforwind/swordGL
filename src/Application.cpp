#include<SDL2/SDL.h>
#include<GL/glew.h>
#include"Application.h"
#include"SDLSupport.h"
#include"LogManager.h"
#include"SwordGLXSupport.h"
#include"shader.hpp"
#include<GL/glew.h>
#include<iostream>
#include<glm/glm.hpp>

Application::Application()
    : sdl_win_(NULL),
      gl_support_(NULL),
      quit_(false),
      use_msaa_(true) {
}

void Application::update(std::chrono::microseconds diff) {
    SDL_Event evt;

    SDL_PollEvent(&evt);
    switch(evt.type) {
    case SDL_QUIT:
        quit_ = true;
        break;
    case SDL_KEYDOWN:
        keyPressed(evt.key);
        break;
    case SDL_MOUSEBUTTONDOWN:
        mousePressed(evt.button);
        break;
    }

    if(!bezier_curve_.empty()) {
        batch_mgr_.addToDynamicBatch(bezier_curve_, GL_LINE_STRIP, GL_STREAM_DRAW, 0/* proId*/);
    }
}
void Application::run() {

    WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                   "start render");

    using namespace std::chrono;
    system_clock::time_point last;
    system_clock::time_point current = std::chrono::system_clock::now();
    microseconds diff;


    glEnable(GL_MULTISAMPLE);
    glPointSize(10);
    glLineWidth(1);
    createSince();
    while(!quit_) {

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        last = current;
        current = std::chrono::system_clock::now();
        diff = duration_cast<microseconds>(current - last);

        update(diff);
        render(diff);
        showFPS(diff);
        //batch_mgr_.cleanDynamicBatch();
        batch_mgr_.resetDynamicBatchVertex();
        gl_support_->swapBuffer();
    }
    destroySince();
}

void Application::showFPS(std::chrono::microseconds diff) {
    static int fps = 0;
    static  std::chrono::microseconds time_since_last_second =
        std::chrono::microseconds(0);

    fps++;
    time_since_last_second += diff;

    if(time_since_last_second >= std::chrono::milliseconds(1000)) {
        SDL_SetWindowTitle(sdl_win_,  std::to_string(fps).c_str());
        time_since_last_second = std::chrono::microseconds(0);
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
    batch_mgr_.addToStaticBatch(vec, GL_POINTS, GL_STATIC_DRAW, proId);
}

void Application::destroySince() {
    WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                   "remove " +
                   std::to_string(batch_mgr_.get_num_of_batch()) +
                   " batchs in Application");

    batch_mgr_.destroyAllBatch();
}

void Application::render(std::chrono::microseconds diff) {
    (void)(diff);
    batch_mgr_.drawAllBatch();
}

void Application::init() {
    WIND::LogManager::initialize();
    WIND::LogManager::set_default_console_logger_format(
        WIND_LOG_TEXT("%D{%H:%M:%S} %p: %m in %M\n"));

    WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                   "-----start init sdl-----");
    SWORD::InitSDLEverything();

    sdl_win_ = SDL_CreateWindow("wind", 0, 0,
                                800, 600,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

#ifdef SWORD_PLANTFORM_LINUX
    gl_support_ = new SWORD::SwordGLXSupport();
#elif defined SWORD_PLANTFORM_WIN
    //TODO---
#endif

    WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                   "-----start init opengl-----");
    std::string sdl_win_handle = SWORD::GetSDLWinHandle(sdl_win_);
    gl_support_->createRenderTarget("wind", "wind", 0, 0, 800, 600, sdl_win_handle);

    WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                   "-----start init glew-----");
    glewExperimental = true;
    GLenum err = glewInit();

    if(err != GLEW_OK) {
        WIND_LOG_ERROR(WIND::LogManager::get_default_console_logger(),
                       glewGetErrorString(err));
        exit(0);
    }

    gl_support_->outputGLInfo();

}

Application::~Application() {
    delete gl_support_;
    SDL_DestroyWindow(sdl_win_);
    SDL_Quit();
}


































