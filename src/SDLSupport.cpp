#include<SDL2/SDL.h>
#include<SDL2/SDL_syswm.h>
#include"SDLSupport.h"
#include"LogManager.h"

SWORD_BEGIN

void InitSDLEverything() {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        WIND_LOG_WARN(WIND::LogManager::get_default_console_logger(),
                      "Can't init SDL2 EVERYING");
    }
}

std::string GetSDLWinHandle(SDL_Window* sdl_window) {
    SDL_SysWMinfo wm_info;

    SDL_VERSION(&wm_info.version);
    if(SDL_GetWindowWMInfo(sdl_window, &wm_info) == SDL_FALSE) {
        WIND_LOG_ERROR(WIND::LogManager::get_default_console_logger(),
                       "Can't get SDL2 window info");
    }
    std::string sdl_handle = "";

    switch(wm_info.subsystem) {
#ifdef SWORD_PLANTFORM_WIN
    case SDL_SYSWM_WINDOWS:
        sdl_handle = std::to_string(static_cast<uint32_t>(wm_info.info.win.window));
        break;
#else
    case SDL_SYSWM_X11:
        sdl_handle = std::to_string(static_cast<uint32_t>(wm_info.info.x11.window));
        break;
#endif
    default:
        WIND_LOG_ERROR(WIND::LogManager::get_default_console_logger(),
                       "UnExcepted Operation System");
        break;
    };
    return sdl_handle;
}
SWORD_END



