#include"RenderWindow.h"
#include<cassert>
#include<SDL2/SDL_video.h>
#include "Singleton.h"

// this class is the root of the engine,

SWORD_BEGIN
class SWORD_EXPORT Root:public Singleton<Root> {
  public:
    Root();
// swapBuffer,call it every frame
// the batch_manager_ will clear the dynamic batch,
// that mean you must set the vertex to the batch every frame
// or set it to the static frame,if they won't changes;
    inline void swapBuffer() {
        assert(render_window_.get_win_handle());
        SDL_GL_SwapWindow(render_window_.get_win_handle());
    }

    inline RenderWindow* get_render_window() {
        return &render_window_;
    }

    ~Root();

	// this method will init sdl ,glew,log4cplus,
	// member variable win_ and context_ will be set,
	// and the render_window_ will attach the win_;
	void init();

  private:

    void createRenderWindow();

    void destroyRenderWindow();

    RenderWindow render_window_;
  
    SDL_GLContext context_;

};
SWORD_END











