/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-03-22 22:21
#
# Filename: SwordGLXSupport.h
#
# Description:
#
==================================================================*/
#include<GL/glx.h>
#include<map>
#include"GLSupport.h"
#include"SwordGLXWindow.h"

SWORD_BEGIN
class SwordGLXSupport: public GLSupport{
  public:
    SwordGLXSupport();

    ~SwordGLXSupport();

    SwordGLXSupport(const SwordGLXSupport& rhs) = delete;

    SwordGLXSupport& operator = (const SwordGLXSupport& rhs) = delete;

    RenderWindow* createRenderTarget(const std::string& ID,
                                     const std::string& title,
                                     int left, int top,
                                     uint32_t width, uint32_t height,
                                     const std::string& parent);

    void destroyRenderTarget(const std::string& ID);

    void swapBuffer();
  private:

    void init();

    std::map<std::string, SwordGLXWindow> render_window_list_;

    Display* displayer_;

    XVisualInfo* visual_info_;

    GLXContext context_;
};
SWORD_END







