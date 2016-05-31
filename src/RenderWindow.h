/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-04-02 19:11
#
# Filename: RenderWindow.h
#
# Description:
#
==================================================================*/

#include"Platform.h"

class SDL_Window;

SWORD_BEGIN

// this class is a wrap for SDL_Window,
// using it instead of SDL_HideWindow,SDL_GetWinSize,etc

class SWORD_EXPORT RenderWindow {
  public:
	RenderWindow();
	~RenderWindow();

    void attach(SDL_Window* win);

	SDL_Window* get_win_handle();

    void set_title(const char* title);

	void set_win_size(const int& width,const int& height);

    void set_visible(bool visable);

    void get_win_size(int& width, int& height);

    void set_fullscreen(bool full);

  private:
    bool vis_;
    bool fullscreen_;
    SDL_Window* win_;
};

SWORD_END







