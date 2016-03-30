#include"Application.h"
#include<SDL2/SDL_events.h>
#include<GL/gl.h>
#include<iostream>
#include"Math.h"

void Application::keyPressed( const SDL_KeyboardEvent &arg ) {

}


void Application::keyReleased(const SDL_KeyboardEvent& arg) {
}

void Application::mouseMoved(const SDL_Event& arg) {
}

void Application::mouseReleased(const SDL_MouseButtonEvent& arg ) {
}

void Application::mousePressed(const SDL_MouseButtonEvent& arg) {
    if(arg.button == SDL_BUTTON_LEFT) {

        SWORD::RenderWindow* win = gl_support_->get_current_render_target();
        if(!win) return;

        int win_width, win_height;
        win->get_win_size(win_width, win_height);

        float x = (2.0f * arg.x - win_width) / win_width;
        float y = -1.0f * (2.0f * arg.y - win_height) / win_height;

        std::vector<glm::vec3> vec = {glm::vec3(x, y, 0.0f)};
        batch_mgr_.addToStaticBatch(vec, GL_POINTS, GL_STATIC_DRAW, 0);

        bezier_control_.push_back(glm::vec3(x, y, 0.0f));
        bezierCurve(bezier_control_, bezier_curve_, 0.001f);

    }
}

void Application::textInput(const SDL_TextInputEvent& arg) {
}


















