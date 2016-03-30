/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-03-27 21:30
#
# Filename: GLSupport.h
#
# Description:
#
==================================================================*/

#ifndef SWORD_GLSUPPORT_H
#define SWORD_GLSUPPORT_H

#include"RenderWindow.h"

SWORD_BEGIN

class SWORD_EXPORT GLSupport {
  public:
    virtual ~GLSupport();

    GLSupport();

    inline RenderWindow* get_current_render_target() {
        return current_render_window_;
    }

    virtual RenderWindow* createRenderTarget(const std::string& ID,
            const std::string& title,
            int left, int top,
            uint32_t width, uint32_t height,
            const std::string& parent) = 0;

    virtual void destroyRenderTarget(const std::string& ID) = 0;

    void outputGLInfo();

    virtual void swapBuffer() = 0;
  protected:
    RenderWindow* current_render_window_;
};


SWORD_END

#endif










