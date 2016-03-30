/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-03-22 09:52
#
# Filename: RenderWindow.h
#
# Description:
#
==================================================================*/
#ifndef SWORD_RENDERWINDOW_H_
#define SWORD_RENDERWINDOW_H_

#include<string>
#include"Platform.h"

SWORD_BEGIN

class SWORD_EXPORT RenderWindow {
  public:
    RenderWindow();

    virtual ~RenderWindow();

    virtual std::string get_hwnd() = 0;

    virtual void set_visible(bool vis) = 0;

    virtual void set_title(const std::string& title) = 0;

    virtual void get_win_size(int& width, int& height) = 0;

  protected:
    virtual void destroy() = 0;

    bool visible_;
};

SWORD_END

#endif //SWORD_RENDERWINDOW_H













