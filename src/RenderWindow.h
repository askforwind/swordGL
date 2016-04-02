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

class SWORD_EXPORT RenderWindow {
  public:
    RenderWindow();

    void attach(SDL_Window* win);

    void set_title(const char* title);

    void set_visible(bool visable);

    void get_win_size(int& width, int& height);

    void set_fullscreen(bool full);

  private:
    bool vis_;
    bool fullscreen_;
    SDL_Window* win_;
};

SWORD_END





