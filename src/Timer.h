#include<SDL2/SDL_timer.h>

class Timer {
  private:
    Uint64 start_;
    Uint64 before_;
    Uint64 current_;
    Uint64 frequency_;
  public:
    Timer()
        : start_(0),
          before_(0),
          current_(0),
          frequency_(0) {
        frequency_ = SDL_GetPerformanceFrequency();
    }
    void begin() {
        before_ = current_ = start_ = SDL_GetPerformanceCounter();
    }
    void tick() {
        before_ = current_;
        current_ = SDL_GetPerformanceCounter();
    }
    float sinceBegin() {
        return (current_ - start_) * 1000.0f / frequency_;
    }
    float sinceLastTick() {
        return (current_ - before_) * 1000.0f / frequency_;
    }

};












