#include"RenderWindow.h"
#include<SDL2/SDL.h>
#include<cassert>
#include "GL/glew.h"
SWORD_BEGIN

RenderWindow::RenderWindow()
    : vis_(false),
      fullscreen_(false),
      win_(0) {}

RenderWindow::~RenderWindow() {
	if (win_) SDL_DestroyWindow(win_);
	win_ = 0;
}

void RenderWindow::attach(SDL_Window* win) {
    win_ = win;
}

SDL_Window * RenderWindow::get_win_handle() {
	return this->win_;
}

void RenderWindow::get_win_pos(int& x, int& y) {
	assert(win_);
	SDL_GetWindowPosition(win_, &x, &y);
}

void RenderWindow::set_title(const char* title) {
    assert(win_);
    SDL_SetWindowTitle(win_, title);
}

void RenderWindow::set_win_size(const int & width,const int & height) {
	assert(win_);
	SDL_SetWindowSize(win_, width, height);
}

void RenderWindow::set_visible(bool visable) {
    assert(win_);
    vis_ = visable;
    vis_ ? SDL_ShowWindow(win_) : SDL_HideWindow(win_);
}

void RenderWindow::set_fullscreen(bool full) {
    assert(win_);
    fullscreen_ = full;

	if (fullscreen_) {
		SDL_MaximizeWindow(win_);
		//SDL_SetWindowFullscreen(win_, SDL_WINDOW_FULLSCREEN);
		int x, y, w, h;
		get_win_pos(x, y);
		get_win_size(w, h);
		glViewport(x, y, w, h);
	}
    else
        SDL_SetWindowSize(win_, 800, 600);
}

void RenderWindow::get_win_size(int& w, int& h) {
    assert(win_);
    SDL_GetWindowSize(win_, &w, &h);
}

SWORD_END













