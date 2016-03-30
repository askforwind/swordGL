#include"SwordGLXWindow.h"
#include"LogManager.h"

SWORD_BEGIN

void SwordGLXWindow::create(const std::string& title,
                            int left, int top,
                            uint32_t width, uint32_t height,
                            const std::string& parent_handle,
                            Display* displayer,
                            XVisualInfo* visual_info) {

    WIND::WindLogger logger;
    logger = WIND::LogManager::get_default_console_logger();

    WIND_LOG_TRACE(logger, "Start Create Window:" + title);

    Window root = parent_handle.empty() ?
                  DefaultRootWindow(displayer) :
                  static_cast<Window>(stoul(parent_handle));


    Colormap cmap = XCreateColormap(displayer, root, visual_info->visual, AllocNone);

    XSetWindowAttributes swatt;
    swatt.colormap = cmap;

    window_ = XCreateWindow(displayer, root, left, top,
                            width, height, 0, visual_info->depth,
                            0, visual_info->visual, CWColormap, &swatt);
    if(!window_) {
        WIND_LOG_ERROR(logger, "Create X Wndow Failed");
        exit(0);
    }
    XStoreName(displayer, window_, title.c_str());

    displayer_ = displayer;
    WIND_LOG_TRACE(logger, "Create Window:" + title + " ok");
}

void SwordGLXWindow::get_win_size(int& width, int& height) {
    XWindowAttributes xwatt;
    XGetWindowAttributes(displayer_, window_, &xwatt);
    width = xwatt.width;
    height = xwatt.height;
}

void SwordGLXWindow::destroy() {
    XDestroyWindow(displayer_, window_);
}

std::string SwordGLXWindow::get_hwnd() {
    return std::to_string(window_);
}

void SwordGLXWindow::set_visible(bool vis) {
    visible_ = vis;

    if(visible_) XMapWindow(displayer_, window_);
    else XUnmapWindow(displayer_, window_);
}

void SwordGLXWindow::set_title(const std::string& title) {
    XStoreName(displayer_, window_, title.c_str());
}

SWORD_END








