#include"BatchManager.h"
#include"ShaderManager.h"
#include"LogManager.h"
#include"RenderWindow.h"
#include<SDL2/SDL.h>


SWORD_BEGIN
class SWORD_EXPORT Root {
  public:
    Root();

    inline void swapBuffer() {
        assert(win_);
        SDL_GL_SwapWindow(win_);
    }

    inline BatchManager* get_batch_manager() {
        return &batch_manager_;
    }

    inline RenderWindow* get_defaule_render_window() {
        return &render_window_;
    }

    ~Root();
  private:
    void init();

    void createRenderWindow();

    void destroyRenderWindow();

    WIND::WindLogger default_log_;

    RenderWindow render_window_;
    SDL_Window* win_;
    SDL_GLContext context_;

    BatchManager batch_manager_;
    ShaderManager shader_manager_;

};
SWORD_END


































