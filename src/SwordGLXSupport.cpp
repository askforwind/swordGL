#include"SwordGLXSupport.h"
#include"SwordGLXWindow.h"
#include"LogManager.h"
#include<GL/gl.h>
#include<GL/glext.h>
#include<GL/glxext.h>
#include<cassert>

SWORD_BEGIN

SwordGLXSupport::SwordGLXSupport()
    : GLSupport() {
    init();
}

void SwordGLXSupport::init() {
    displayer_ = XOpenDisplay(NULL);
    assert(displayer_ != NULL && "displayer can't be create");

    int att[] = { GLX_X_RENDERABLE    , True,
                  GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
                  GLX_RENDER_TYPE     , GLX_RGBA_BIT,
                  GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
                  GLX_RED_SIZE        , 8,
                  GLX_GREEN_SIZE      , 8,
                  GLX_BLUE_SIZE       , 8,
                  GLX_ALPHA_SIZE      , 8,
                  GLX_DEPTH_SIZE      , 24,
                  GLX_STENCIL_SIZE    , 8,
                  GLX_DOUBLEBUFFER    , True,
                  GLX_SAMPLE_BUFFERS  , 1,            // <-- MSAA
                  GLX_SAMPLES         , 4,            // <-- MSAA
                  None
                };

    GLXFBConfig fbconfig = 0;
    int fbcount = 0;
    GLXFBConfig* fbc = glXChooseFBConfig(displayer_,
                                         XDefaultScreen(displayer_),
                                         att, &fbcount);
    assert(fbc && fbcount > 0 && "create MSAA fbcount failed");

    fbconfig = fbc[0];
    XFree(fbc);

    visual_info_ = glXGetVisualFromFBConfig(displayer_, fbconfig);

    assert(visual_info_ != NULL && "visual info can't be create");

    context_ = glXCreateContext(displayer_, visual_info_, NULL, GL_TRUE);

}

SwordGLXSupport::~SwordGLXSupport() {
    if(glXMakeCurrent(displayer_, None, NULL) == false) {
        WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                       "Can't make stop glcontext connection");
    }

    WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                   "delete the rest " + std::to_string(render_window_list_.size()) + " window");
    glXDestroyContext(displayer_, context_);

    for(auto i = render_window_list_.begin();
            i != render_window_list_.end();
            ++i) {
        i->second.destroy();
    }
    XCloseDisplay(displayer_);
}

RenderWindow* SwordGLXSupport::createRenderTarget(const std::string& ID,
        const std::string& title,
        int left, int top,
        uint32_t width, uint32_t height,
        const std::string& parent) {
    if(render_window_list_.find(ID) == render_window_list_.end()) {

        render_window_list_[ID].create(title, left, top,
                                       width, height,
                                       parent,
                                       displayer_, visual_info_);

        if(render_window_list_.size() == 1) {
            current_render_window_ = &render_window_list_[ID];
            current_render_window_->set_visible(true);
            glXMakeCurrent(displayer_,
                           static_cast<Window>(stoul(current_render_window_->get_hwnd())),
                           context_);
        }

        return &render_window_list_[ID];
    } else {
        WIND_LOG_WARN(WIND::LogManager::get_default_console_logger(),
                      "Can't create Render Window with same ID:" + ID);
        return NULL;
    }
}

void SwordGLXSupport::destroyRenderTarget(const std::string& ID) {

    auto iter = render_window_list_.find(ID);

    if(iter != render_window_list_.end()) {
        WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                       "destroy window :" + ID);

        if(&(iter->second) == current_render_window_) {
            WIND_LOG_WARN(WIND::LogManager::get_default_console_logger(),
                          "destroy the Current Render Window:" +
                          ID +
                          " please set new window to Render");
            glXMakeCurrent(displayer_, None, NULL);
            current_render_window_ = NULL;

        }
        iter->second.destroy();
        render_window_list_.erase(iter);
    } else {
        WIND_LOG_WARN(WIND::LogManager::get_default_console_logger(),
                      "can't destroy window:" + ID + " can't find it");
    }

}

void SwordGLXSupport::swapBuffer() {
    glXSwapBuffers(displayer_,
                   static_cast<Window>(stoul(current_render_window_->get_hwnd())));
}
SWORD_END














































