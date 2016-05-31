
#include "test.h"



bool wasd[6] = { false, false, false, false,false,false };

bool output_Pos = false;
int shipDir = 1;

void keyPress(const SDL_KeyboardEvent &arg) {
	if (arg.keysym.sym == SDLK_w) wasd[0] = true;
	if (arg.keysym.sym == SDLK_a) wasd[1] = true;
	if (arg.keysym.sym == SDLK_s) wasd[2] = true;
	if (arg.keysym.sym == SDLK_d) wasd[3] = true;
	if (arg.keysym.sym == SDLK_q) wasd[4] = true;
	if (arg.keysym.sym == SDLK_e) wasd[5] = true;
	if (arg.keysym.sym == SDLK_o) output_Pos = true;
	if (arg.keysym.sym == SDLK_f) shipDir *= -1;
}

void keyRelease(const SDL_KeyboardEvent& arg) {
	if (arg.keysym.sym == SDLK_w) wasd[0] = false;
	if (arg.keysym.sym == SDLK_a) wasd[1] = false;
	if (arg.keysym.sym == SDLK_s) wasd[2] = false;
	if (arg.keysym.sym == SDLK_d) wasd[3] = false;
	if (arg.keysym.sym == SDLK_q) wasd[4] = false;
	if (arg.keysym.sym == SDLK_e) wasd[5] = false;
	if (arg.keysym.sym == SDLK_o) output_Pos = false;
}

bool toMoveMouse = false;

void moveMouse(const SDL_MouseMotionEvent& arg) {
	toMoveMouse = true;
}

float speed = 0.2f;
void update(SWORD::FPSCamera& cam, float dt) {
	
	int z = wasd[2] - wasd[0];
	int	x = wasd[3] - wasd[1];
	int y = wasd[4] - wasd[5];
	glm::vec3 d(0.0f);
	glm::vec3 r(0.0f);
	glm::vec3 u(0.0f);
	if (x) d = 1.0f*x*cam.get_right();
	if (z) r = -1.0f*z*cam.get_direction();
	if (y) u = 1.0f*y*cam.get_up();
	if (x || z || y) cam.translate(glm::normalize(d + r + u)*dt*speed);

	if (toMoveMouse) {
		int ox, oy;
		SDL_GetMouseState(&ox, &oy);
		int w, h;
		g_root.get_render_window()->get_win_size(w, h);
		cam.moveMouse(w / 2.0f - ox, h / 2.0f - oy, 0.001f);
		SDL_WarpMouseInWindow(g_root.get_render_window()->get_win_handle(), w / 2.0f, h / 2.0f);
		SDL_PumpEvents();
		SDL_FlushEvent(SDL_MOUSEMOTION);
	}
	toMoveMouse = false;

	glm::vec3 p = cam.get_position();
	if (output_Pos) std::cout << p.x << " " << p.y << " " << p.z << std::endl;

}

void updateShip(SWORD::Entity* ship,SWORD::Entity* fuck,float time)
{
	float s = time*shipDir;
	ship->translate(glm::vec3(0.01f, 0.0f, 0.0f)*s, SWORD::Coord::LOCAL);
	fuck->translate(glm::vec3(0.01f, 0.0f, 0.0f)*s, SWORD::Coord::LOCAL);
}

int main(int argc, char*argv []) {
	g_root.init();
	g_root.get_render_window()->set_fullscreen(true);
	SDL_WarpMouseInWindow(g_root.get_render_window()->get_win_handle(), 400, 300);
	SWORD::FPSCamera mCamera;
	initCamera(&mCamera);

	
	assert(glGetError()==0);
	
	SWORD::TechniquePtr scene_tech = initSceneTech();
	SWORD::TechniquePtr tech = initWarriorTech();
	initDirectonLight(scene_tech, "light", 1);
	bindTechToBlock(tech, "light", 1);

	SWORD::EntityManager eMgr(&g_Combiner, &mCamera);

	SWORD::ModulePtr module = SWORD::ResourceGroup::instance().loadModule("D:/swordGL/resource/war_fbx.FBX");
	SWORD::Entity* et = eMgr.creatEntity(tech, module);

	SWORD::ModulePtr ship = SWORD::ResourceGroup::instance().loadModule("D:/swordGL/resource/ship.fbx");
	SWORD::Entity* p_ship = eMgr.creatEntity(scene_tech, ship);

	SWORD::ModulePtr fuck = SWORD::ResourceGroup::instance().loadModule("D:/swordGL/resource/boblampclean.md5mesh");
	SWORD::Entity* p_fuck = eMgr.creatEntity(tech, fuck);

	SWORD::ModulePtr scene = SWORD::ResourceGroup::instance().loadModule("D:/swordGL/resource/newcity.fbx");
	eMgr.creatEntity(scene_tech, scene);

	p_ship->set_pos(glm::vec3(927.355f, 1000.344f, -43.9387f),SWORD::Coord::WORLD);
	p_fuck->set_pos(glm::vec3(927.355f, 1080.344f, -43.9387f), SWORD::Coord::WORLD);
	bool quit = false;
	SDL_ShowCursor(0);

	SDL_Event evt;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	Timer time;
	time.begin();

	int fps_count = 0;
	float fps_time = 0.0f;
	
	SWORD::RenderQueue RQ;
	
	assert(glGetError() == 0);

	while (!quit) {
		
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		while (SDL_PollEvent(&evt)) {
			switch (evt.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYUP:
				keyRelease(evt.key);
				break;
			case SDL_KEYDOWN:
				keyPress(evt.key);
				break;
			case SDL_MOUSEMOTION:
				moveMouse(evt.motion);
			default:break;
			}
		}

		update(mCamera, time.sinceLastTickInMillSecond());
		updateShip(p_ship, p_fuck, time.sinceBeginInSecond());

		fps_time += time.sinceLastTickInMillSecond();
		fps_count++;

		if (fps_time > 1000.0f) {
			g_root.get_render_window()->set_title(
				std::to_string(fps_count).c_str());
			fps_count = 0;
			fps_time = 0.0f;
		}

		time.tick();
		assert(glGetError() == 0);

		SWORD::MultiDrawCallCommandVec mdcc;

		eMgr.generateDrawCallQueue(mdcc);

		SWORD::RenderQueue RQ;

		

		for (size_t id = 0; id != mdcc.size(); ++id) {
			RQ.inputMultiDrawCall(std::move(mdcc[id]));
		}

		RQ.render();
		RQ.cleanDrawCallQueue();
		g_root.swapBuffer();
	}

	glBindVertexArray(0);

	return 0;
}


