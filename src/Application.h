#include"Root.h"
#include"Timer.h"
#include<vector>

class SDL_Window;
union SDL_Event;
struct SDL_MouseButtonEvent;

struct SDL_TextInputEvent;
struct SDL_KeyboardEvent;

class Application {
  public:
    Application();
    ~Application() ;

    void update(float diff);

    void createSince();

    void destroySince();

    void run();

    void render(float diff);
  private:
    std::vector<glm::vec3> bezier_control_;
    std::vector<glm::vec3> bezier_curve_;

    void mouseMoved(const SDL_Event& arg) ;
    void mousePressed(const SDL_MouseButtonEvent& arg) ;
    void mouseReleased(const SDL_MouseButtonEvent& arg) ;
    void textInput( const SDL_TextInputEvent& arg ) ;
    void keyPressed( const SDL_KeyboardEvent &arg ) ;
    void keyReleased (const SDL_KeyboardEvent &arg );

    void showFPS(float diff);
    SDL_Event evt_;
    bool quit_;
    bool use_msaa_;
    SWORD::Root* root_;
    Timer timer_;
};






















