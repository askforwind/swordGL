#include"Application.h"
#include<GL/glew.h>
#include"shader.hpp"
#include"LogManager.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

Application::Application()
    : quit_(false),
      use_msaa_(true) {
    root_ = new SWORD::Root;
}

Application::~Application() {
    delete root_;
}

void Application::update(float d) {
    SDL_Event evt;
    SDL_PollEvent(&evt);
    switch(evt.type) {
    case SDL_QUIT:
        quit_ = true;
        break;
    case SDL_KEYDOWN:
        keyPressed(evt.key);
        break;
    case SDL_MOUSEBUTTONDOWN:
        mousePressed(evt.button);
        break;
    }

    if(!bezier_curve_.empty()) {
        root_->get_batch_manager()->addToDynamicBatch(bezier_curve_, GL_LINE_STRIP, GL_STREAM_DRAW, 0/* proId*/);
    }
}
void Application::run() {

    WIND_LOG_TRACE(DEFAULT_WIND_LOGGER, "start render");

    glEnable(GL_MULTISAMPLE);
    glPointSize(10);
    glLineWidth(1);
    createSince();

    timer_.begin();
    while(!quit_) {

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        timer_.tick();
        update(timer_.sinceLastTick());
        render(timer_.sinceLastTick());
        showFPS(timer_.sinceLastTick());
        root_->swapBuffer();
    }
    destroySince();
}

void Application::showFPS(float diff) {
    static int fps = 0;
    static float time_since_last_second = 0.0f;
    fps++;
    time_since_last_second += diff;

    if(time_since_last_second >= 1000.0f) {
        root_->get_render_window()->set_title(std::to_string(fps).c_str());
        time_since_last_second = 0.0f;
        fps = 0;
    }
}

void Application::createSince() {
    WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                   "start create since");
    uint32_t proId = LoadShaders( "SimpleVertexShader.glsl", "SimpleFragmentShader.glsl" );
    int32_t matId = glGetUniformLocation(proId, "MVP");
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
                         glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
                         glm::vec3(0, 0, 0), // and looks at the origin
                         glm::vec3(0, 1, 0) // Head is up (set to 0,-1,0 to look upside-down)
                     );
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = projection * view * model;
    glUseProgram(proId);
    glUniformMatrix4fv(matId, 1, GL_FALSE, &mvp[0][0]);
    glm::vec3 s = {1.0, 2.0, 3.0};
    std::vector<glm::vec3> vec = {{ -1.0f, -1.0f, -1.0f}, // triangle 1 : begin
        { -1.0f, -1.0f, 1.0f},
        { -1.0f, 1.0f, 1.0f}, // triangle 1 : end
        { 1.0f, 1.0f, -1.0f}, // triangle 2 : begin
        {        -1.0f, -1.0f, -1.0f},
        {       -1.0f, 1.0f, -1.0f}, // triangle 2 : end
        {      1.0f, -1.0f, 1.0f},
        {     -1.0f, -1.0f, -1.0f},
        {    1.0f, -1.0f, -1.0f},
        {   1.0f, 1.0f, -1.0f},
        {  1.0f, -1.0f, -1.0f},
        { -1.0f, -1.0f, -1.0f},
        {            -1.0f, -1.0f, -1.0f},
        {             -1.0f, 1.0f, 1.0f},
        {              -1.0f, 1.0f, -1.0f},
        {           1.0f, -1.0f, 1.0f},
        {               -1.0f, -1.0f, 1.0f},
        {                -1.0f, -1.0f, -1.0f},
        {                 -1.0f, 1.0f, 1.0f},
        {                  -1.0f, -1.0f, 1.0f},
        {                   1.0f, -1.0f, 1.0f},
        {                    1.0f, 1.0f, 1.0f},
        {                     1.0f, -1.0f, -1.0f},
        {                      1.0f, 1.0f, -1.0f},
        {                       1.0f, -1.0f, -1.0f},
        {                        1.0f, 1.0f, 1.0f},
        {                         1.0f, -1.0f, 1.0f},
        {                          1.0f, 1.0f, 1.0f},
        {                           1.0f, 1.0f, -1.0f},
        {                            -1.0f, 1.0f, -1.0f},
        {                             1.0f, 1.0f, 1.0f},
        {                              -1.0f, 1.0f, -1.0f},
        {                               -1.0f, 1.0f, 1.0f},
        {                                1.0f, 1.0f, 1.0f},
        {                                 -1.0f, 1.0f, 1.0f},
        {                                  1.0f, -1.0f, 1.0f}
    };
    GLenum err = glGetError();
    if(err != GL_NO_ERROR) {
        WIND_LOG_ERROR(DEFAULT_WIND_LOGGER, gluErrorString(err));
        exit(0);
    }

    root_->get_batch_manager()->addToStaticBatch(vec, GL_TRIANGLE_STRIP, GL_STATIC_DRAW, proId);
}

void Application::destroySince() {
    WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                   "remove " +
                   std::to_string(root_->get_batch_manager()->get_num_of_batch()) +
                   " batchs in Application");

    root_->get_batch_manager()->destroyAllBatch();
}

void Application::render(float diff) {
    (void)(diff);
    root_->get_batch_manager()->drawAllBatch();
}























































































