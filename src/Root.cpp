#include"Root.h"
#include"LogManager.h"
#include<FreeImage.h>
#include<GL/glew.h>
#include<SDL2/SDL.h>

SWORD_BEGIN

Root::Root()
    : win_(0),
      context_(0) {
    init();
}

Root::~Root() {
    destroyRenderWindow();
    SDL_Quit();
    FreeImage_DeInitialise();
}

void Root::init() {
    //---------init log4cplus-----------
    WIND::LogManager::initialize();
    WIND::LogManager::set_default_console_logger_format(
        WIND_LOG_TEXT("%D{%H:%M:%S} %p: %m in %M %L\n"));
    WIND::LogManager::set_default_file_logger_format(
        WIND_LOG_TEXT("%D{%H:%M:%S} %p: %m in %M %L\n"));

    //---------init SDL2----------------
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        WIND_LOG_ERROR(DEFAULT_WIND_LOGGER, SDL_GetError());
        exit(0);
    }

    //---------init FreeImage----------
    FreeImage_Initialise();
    FreeImage_SetOutputMessage(
    [](FREE_IMAGE_FORMAT fif, const char* message)->void {
        std::string old_console_format = WIND::LogManager::
        set_default_console_logger_format("%D{%H:%M:%S} %p: %m\n");
        std::string old_file_format = WIND::LogManager::
        set_default_file_logger_format("%D{%H:%M:%S} %p: %m\n");

        std::string error = "Image Error:";
        WIND_LOG_ERROR(DEFAULT_WIND_LOGGER,
        error
        + FreeImage_GetFormatFromFIF(fif)
        + " " + message);

        WIND::LogManager::set_default_console_logger_format(old_console_format);
        WIND::LogManager::set_default_file_logger_format(old_file_format);
    });
    // create Render Window,and glcontext
    createRenderWindow();

    //---------init glew after create Render Window---
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
    }

    WIND_LOG_TRACE(DEFAULT_WIND_LOGGER, glGetString(GL_VERSION));
}

void Root::createRenderWindow() {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    win_ = SDL_CreateWindow("OpenGL Window",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!win_) {
        WIND_LOG_ERROR(DEFAULT_WIND_LOGGER, SDL_GetError());
        exit(0);
    }

    context_ = SDL_GL_CreateContext(win_);
    if(!context_) {
        WIND_LOG_ERROR(DEFAULT_WIND_LOGGER, SDL_GetError());
    }
    render_window_.attach(win_);
}

void Root::destroyRenderWindow() {
    SDL_DestroyWindow(win_);
    SDL_GL_DeleteContext(context_);
    render_window_.attach(0);
}

SWORD_END





























































