#include<GL/glew.h>
#include"Root.h"
#include"shader.hpp"
#include"FPSCamera.h"
#include"ResourceGroup.h"
#include<iostream>
#include"SDL2\SDL_mouse.h"
#include"Timer.h"

int32_t matId;
SWORD::Root root;

float speed = 0.05;
bool wasd[4] = { false, false, false, false };

void keyPress(const SDL_KeyboardEvent &arg) {
	if (arg.keysym.sym == SDLK_w) wasd[0] = true;
	if (arg.keysym.sym == SDLK_a) wasd[1] = true;
	if (arg.keysym.sym == SDLK_s) wasd[2] = true;
	if (arg.keysym.sym == SDLK_d) wasd[3] = true;
}

void keyRelease(const SDL_KeyboardEvent& arg) {
	if (arg.keysym.sym == SDLK_w) wasd[0] = false;
	if (arg.keysym.sym == SDLK_a) wasd[1] = false;
	if (arg.keysym.sym == SDLK_s) wasd[2] = false;
	if (arg.keysym.sym == SDLK_d) wasd[3] = false;
}

void update(SWORD::FPSCamera& cam) {

	int ox, oy;
	SDL_GetMouseState(&ox, &oy);
	float offx = 400.0f - ox;
	float offy = 300.0f - oy;
	
	cam.moveMouse(offx, offy, 0.005f);

	int z = wasd[2] - wasd[0];
	int	x = wasd[3] - wasd[1];

	glm::vec3 dir = cam.get_direction();
	glm::vec3 right = glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 d(0.0f);
	if (x) d = (float)x * right;
	if (z) d += -(float)z*dir;
	if (x || z) cam.translate(glm::normalize(d)*speed);

	SDL_WarpMouseInWindow(SDL_GetMouseFocus(), 400, 300);

	glm::mat4 mvp = cam.get_matrix()*glm::mat4(1.0f);
	glUniformMatrix4fv(matId, 1, GL_FALSE, &mvp[0][0]);
}

struct fuck {
	uint32_t ver, idx, uv;
};

int main(int argc,char*argv[]) {
	root.init();
	SWORD::ResourceGroup mResource;
	mResource.initialise();
	SDL_WarpMouseInWindow(root.get_render_window()->get_win_handle(), 400, 300);
	SWORD::FPSCamera mCamera;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	mCamera.set_clip_plane(0.1f, 1000.0f);
	mCamera.set_aspect(4.0f / 3.0f);
	
	mCamera.set_position(glm::vec3(0, 0, 50));
	mCamera.lookAt(glm::vec3(0, 0, 0));

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mvp = mCamera.get_matrix()*  model;
	uint32_t proId = LoadShaders("vs.glsl", "ps.glsl");

    glUseProgram(proId);
	matId = glGetUniformLocation(proId, "MVP");
	//glm::mat4 mvp = mCamera.get_matrix()*glm::mat4(1.0f);
    glUniformMatrix4fv(matId, 1, GL_FALSE, &mvp[0][0]);
	int32_t  tex0 = glGetUniformLocation(proId, "tex0");
    
	SWORD::Texture* tex = mResource.loadTexture("t.png", "t");
	SWORD::Module* mod = mResource.loadModule("F:/resource/warrior/war_fbx.FBX", "cube");

	std::vector<fuck> fff;
	fff.resize(mod->size());

    uint32_t elevao;
    glGenVertexArrays(1, &elevao);
    glBindVertexArray(elevao);

	for (int i = 0; i < mod->size(); ++i) {

		const SWORD::Mesh* mesh = mResource.get_mesh(mod->operator[](i));
		const SWORD::Mesh::Info* info = mesh->get_info();

		glGenBuffers(1, &fff[i].idx);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fff[i].idx);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t)*info->index.size(),
					 info->index.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &fff[i].ver);
		glBindBuffer(GL_ARRAY_BUFFER, fff[i].ver);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*info->vertex.size(),
					 info->vertex.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &fff[i].uv);
		glBindBuffer(GL_ARRAY_BUFFER, fff[i].uv);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*info->uv.size(),
					 info->uv.data(), GL_STATIC_DRAW);
	}

    bool quit = false;
    SDL_Event evt;
    glEnable(GL_DEPTH_TEST);
	Timer time;
	time.begin();
	time.tick();
    while(!quit) {
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
			default:break;
			}
		}
		update(mCamera);
        glEnableVertexAttribArray(0);

        tex->bindToActiveUnit(1);
        glUniform1i(tex0, 1);
		for (int i = 0; i < mod->size(); i++) {
			
			glBindBuffer(GL_ARRAY_BUFFER, fff[i].ver);
			glVertexAttribPointer(
				0,                  // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, fff[i].uv);
			glVertexAttribPointer(
				1,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
			);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fff[i].idx);

			const SWORD::Mesh* mesh = mResource.get_mesh(mod->operator[](i));
			const SWORD::Mesh::Info* info = mesh->get_info();
			const SWORD::Material::Info* mf = mResource.get_material(info->material_id)->get_info();
			const SWORD::Texture* d = mResource.get_texture(mf->tex_diff);
			if (d != 0) {
				d->bindToActiveUnit(1);
				glUniform1i(tex0, 1);
			}

			glDrawElements(GL_TRIANGLES,
						   info->index.size(), GL_UNSIGNED_SHORT, 0);
		}
       
        root.swapBuffer();
    }
	tex->unload();
	mResource.deinitialise();
    glBindVertexArray(0);
    glDeleteProgram(proId);
    glDeleteVertexArrays(1, &elevao);
    return 0;
}
























































































































