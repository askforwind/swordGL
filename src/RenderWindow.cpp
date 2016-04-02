#include"RenderWindow.h"
#include<SDL2/SDL.h>
#include<cassert>

SWORD_BEGIN

RenderWindow::RenderWindow()
    : vis_(false),
      fullscreen_(false),
      win_(0) {}

void RenderWindow::attach(SDL_Window* win) {
    win_ = win;
}

void RenderWindow::set_title(const char* title) {
    assert(win_);
    SDL_SetWindowTitle(win_, title);
}

void RenderWindow::set_visible(bool visable) {
    assert(win_);
    vis_ = visable;
    vis_ ? SDL_ShowWindow(win_) : SDL_HideWindow(win_);
}

void RenderWindow::set_fullscreen(bool full) {
    assert(win_);
    fullscreen_ = full;

    if(fullscreen_)
        SDL_SetWindowFullscreen(win_, 0);
    else
        SDL_SetWindowSize(win_, 800, 600);
}

void RenderWindow::get_win_size(int& w, int& h) {
    assert(win_);
    SDL_GetWindowSize(win_, &w, &h);
}

SWORD_END













