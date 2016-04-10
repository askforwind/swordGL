#include"Application.h"
#include<SDL2/SDL_events.h>
#include<GL/glew.h>
#include"Math.h"
#include"LogManager.h"
#include<iostream>
void Application::keyPressed( const SDL_KeyboardEvent &arg ) {
    if(arg.keysym.sym == SDLK_m) {
        use_msaa_ = !use_msaa_;
        use_msaa_ ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);
        WIND_LOG_TRACE(WIND::LogManager::get_default_console_logger(),
                       "Use Msaa" + std::to_string(use_msaa_));
    }
    if(arg.keysym.sym == SDLK_c) {
        bezier_curve_.clear();
        bezier_control_.clear();
        root_->get_batch_manager()->cleanAllBatch();
    }
}


void Application::keyReleased(const SDL_KeyboardEvent& arg) {
}

void Application::mouseMoved(const SDL_Event& arg) {
}

void Application::mouseReleased(const SDL_MouseButtonEvent& arg ) {
}

void Application::mousePressed(const SDL_MouseButtonEvent& arg) {
    if(arg.button == SDL_BUTTON_LEFT) {

        SWORD::RenderWindow* win = root_->get_defaule_render_window();
        if(!win) return;

        int win_width, win_height;
        win->get_win_size(win_width, win_height);

        float x = (2.0f * arg.x - win_width) / win_width;
        float y = -1.0f * (2.0f * arg.y - win_height) / win_height;

        std::vector<glm::vec3> vec = {glm::vec3(x, y, 0.0f)};
        root_->get_batch_manager()->addToStaticBatch(vec, GL_POINTS, GL_STATIC_DRAW, 0);

        bezier_control_.push_back(glm::vec3(x, y, 0.0f));
        bezierCurve(bezier_control_, bezier_curve_, 0.001f);

        std::cout << bezier_control_.size() << " " << bezier_curve_.size() << std::endl;
    }
}

void Application::textInput(const SDL_TextInputEvent& arg) {
}





























