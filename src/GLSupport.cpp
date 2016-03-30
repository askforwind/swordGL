#include"GLSupport.h"
#include"LogManager.h"
#include<GL/gl.h>
SWORD_BEGIN

GLSupport::GLSupport()
    : current_render_window_(0) {
}

void GLSupport::outputGLInfo() {
    std::string info_title = "------GL INFO-----\n";
    int bufs, samples;

    glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
    glGetIntegerv(GL_SAMPLES, &samples);

    WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                   info_title +
                   "           ------" + (const char*)glGetString(GL_VERSION) + "------\n" +
                   "           ------MSAA: buffers = " + std::to_string(bufs) +
                   " samples = " + std::to_string(samples) + "------\n"
                  );
}

GLSupport::~GLSupport() {
}

SWORD_END


