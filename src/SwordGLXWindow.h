/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-03-22 10:33
#
# Filename: SwordGLXWindow.h
#
# Description:
#
==================================================================*/
#ifndef SWORD_GLXWindow_H_
#define SWORD_GLXWindow_H_

#include<GL/glx.h>
#include"RenderWindow.h"

SWORD_BEGIN

class SwordGLXSupport;

class SwordGLXWindow: public RenderWindow {
  public:
    friend SwordGLXSupport;

    virtual std::string get_hwnd();
     
    virtual void set_visible(bool vis);
    
    virtual void set_title(const std::string& title);

    virtual void get_win_size(int& width,int& height);

  private:
    virtual void destroy() ;

    void create(const std::string& title,
                int left, int top,
                uint32_t width, uint32_t height,
                const std::string& parent_handle, Display* displayer,
                XVisualInfo* visual_info) ;

    Display* displayer_;
    ::Window window_;
};
SWORD_END

#endif



