
#ifndef TIMER_H__
#define TIMER_H__

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
    float sinceBeginInSecond() {
        return 1.0f * (current_ - start_)  / frequency_;
    }
	
	float sinceBeginInMilliSecond(){ 
		return 1000.0f * (current_ - start_)  / frequency_;
	}

    float sinceLastTickInSecond() {
		return 1.0f * (current_ - before_) / frequency_;
    }

	float sinceLastTickInMillSecond(){
		return 1000.0f * (current_ - before_) / frequency_;
	}


};

#endif // TIMER_H__










