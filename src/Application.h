#include"GLSupport.h"
#include"BatchManager.h"
#include<chrono>
#include<vector>

class SDL_Window;
union SDL_Event;
struct SDL_MouseButtonEvent;

struct SDL_TextInputEvent;
struct SDL_KeyboardEvent;

class Application {
  public:
    Application();
    ~Application();

    void init();

    void update(std::chrono::microseconds diff);

    void createSince();

    void destroySince();

    void run();

    void render(std::chrono::microseconds diff);
  private:
    std::vector<glm::vec3> bezier_control_;
    std::vector<glm::vec3> bezier_curve_;

    void mouseMoved(const SDL_Event& arg) ;
    void mousePressed(const SDL_MouseButtonEvent& arg) ;
    void mouseReleased(const SDL_MouseButtonEvent& arg) ;
    void textInput( const SDL_TextInputEvent& arg ) ;
    void keyPressed( const SDL_KeyboardEvent &arg ) ;
    void keyReleased (const SDL_KeyboardEvent &arg );

    void showFPS(std::chrono::microseconds diff);

    SDL_Window* sdl_win_;
    SWORD::GLSupport* gl_support_;
    SWORD::BatchManager batch_mgr_;

/*    uint32_t proId;*/
    /*uint32_t shader_mvp_;*/
    bool quit_;
    bool use_msaa_;
};














