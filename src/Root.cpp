#include"Root.h"
#include"LogManager.h"
#include<GL/glew.h>
#include<SDL2/SDL.h>

SWORD_BEGIN

Root* Root::msSingleton = nullptr;

Root::Root()
    :context_(0) {
}

Root::~Root() {
    destroyRenderWindow();
    SDL_Quit();
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
        exit(-1);
    }

   
    // create Render Window,and glcontext
    createRenderWindow();

    //---------init glew after create Render Window---
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
		exit(-1);
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
	/*SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);*/
    SDL_Window* win = SDL_CreateWindow("OpenGL Window",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	int k = 0;
	SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &k);
    if (!win) {
        WIND_LOG_ERROR(DEFAULT_WIND_LOGGER, SDL_GetError());
        exit(-1);
    }
    context_ = SDL_GL_CreateContext(win);
    if(!context_) {
        WIND_LOG_ERROR(DEFAULT_WIND_LOGGER, SDL_GetError());
		exit(-1);
    }
    render_window_.attach(win);
}

void Root::destroyRenderWindow() {
	SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(render_window_.get_win_handle());
    SDL_GL_DeleteContext(context_);
    render_window_.attach(0);
}

SWORD_END





























































