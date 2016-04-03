#include"BatchManager.h"
#include"ShaderManager.h"
#include"RenderWindow.h"
#include<SDL2/SDL.h>

// this class is the root of the engine,

SWORD_BEGIN
class SWORD_EXPORT Root {
  public:
    Root();
// swapBuffer,call it every frame
// the batch_manager_ will clear the dynamic batch,
// that mean you must set the vertex to the batch every frame
// or set it to the static frame,if they won't changes;
    inline void swapBuffer() {
        assert(win_);
        SDL_GL_SwapWindow(win_);
        batch_manager_.cleanDynamicBatch();
    }

    inline BatchManager* get_batch_manager() {
        return &batch_manager_;
    }

    inline RenderWindow* get_defaule_render_window() {
        return &render_window_;
    }

    ~Root();
  private:
    // this method will init sdl ,glew,log4cplus,
    // member variable win_ and context_ will be set,
    // and the render_window_ will attach the win_;
    void init();

    void createRenderWindow();

    void destroyRenderWindow();

    RenderWindow render_window_;
    SDL_Window* win_;
    SDL_GLContext context_;

    BatchManager batch_manager_;
    ShaderManager shader_manager_;

};
SWORD_END











