#include<GL/glew.h>
#include"Root.h"
#include"shader.h"
#include"FPSCamera.h"
#include"ResourceGroup.h"
#include<iostream>
#include"SDL2\SDL_mouse.h"
#include"Timer.h"
#include "SDL2/SDL_gamecontroller.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "assimp\scene.h"
#include "glm/gtx/quaternion.hpp"
#include "glm\glm.hpp"
#include <set>
#include "LogManager.h"
#include "config.h"
#include "ResourceDef.h"
#include "Texture.h"
#include <fstream>
#include "Skeleton.h"
#include "../build/rightAnim.h"

int32_t matId;
SWORD::Root root;


bool wasd[6] = { false, false, false, false,false,false };

void keyPress(const SDL_KeyboardEvent &arg) {
	if (arg.keysym.sym == SDLK_w) wasd[0] = true;
	if (arg.keysym.sym == SDLK_a) wasd[1] = true;
	if (arg.keysym.sym == SDLK_s) wasd[2] = true;
	if (arg.keysym.sym == SDLK_d) wasd[3] = true;
	if (arg.keysym.sym == SDLK_q) wasd[4] = true;
	if (arg.keysym.sym == SDLK_e) wasd[5] = true;
	//std::cout << wasd[0] << wasd[1] << wasd[2] << wasd[3] << std::endl;
}

void keyRelease(const SDL_KeyboardEvent& arg) {
	if (arg.keysym.sym == SDLK_w) wasd[0] = false;
	if (arg.keysym.sym == SDLK_a) wasd[1] = false;
	if (arg.keysym.sym == SDLK_s) wasd[2] = false;
	if (arg.keysym.sym == SDLK_d) wasd[3] = false;
	if (arg.keysym.sym == SDLK_q) wasd[4] = false;
	if (arg.keysym.sym == SDLK_e) wasd[5] = false;
	//std::cout << wasd[0] << wasd[1] << wasd[2] << wasd[3] << std::endl;
}
float speed = 0.02;
void update(SWORD::FPSCamera& cam, float dt) {

	int ox, oy;
	//std::cout << dt << std::endl;
	int z = wasd[2] - wasd[0];
	int	x = wasd[3] - wasd[1];
	int y = wasd[4] - wasd[5];
	glm::vec3 d(0.0f);
	glm::vec3 r(0.0f);
	glm::vec3 u(0.0f);
	if (x) d = 1.0f*x*cam.get_right();
	if (z) r = -1.0f*z*cam.get_direction();
	if (y) u = 1.0f*y*cam.get_up();
	//std::cout << d.x << d.y << d.z << std::endl;
	if (x || z || y) cam.translate(glm::normalize(d + r + u)*dt*speed);

	SDL_GetMouseState(&ox, &oy);
	float offx = 400.0f - ox;
	float offy = 300.0f - oy;
	cam.yaw(offx*0.001);
	cam.pitch(offy*0.001);
	SDL_WarpMouseInWindow(root.get_render_window()->get_win_handle(), 400, 300);
	//SDL_PumpEvents();
	SDL_FlushEvent(SDL_MOUSEMOTION);
	glm::mat4 mvp = cam.get_matrix()*glm::mat4(1.0f);
	glUniformMatrix4fv(matId, 1, GL_FALSE, &mvp[0][0]);
}

int main(int argc, char*argv []) {
	root.init();
	SDL_WarpMouseInWindow(root.get_render_window()->get_win_handle(), 400, 300);
	SWORD::FPSCamera mCamera;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	mCamera.set_clip_plane(0.1f, 10000.0f);
	mCamera.set_aspect(4.0f / 3.0f);
	mCamera.set_position(glm::vec3(0, 0, 60));
	mCamera.lookAt(glm::vec3(0, 0, 0));
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mvp = mCamera.get_matrix()*  model;

	Technique tech;
	tech.Init();
	if(tech.AddShader(GL_VERTEX_SHADER, "vs.glsl")!=true) assert(0);
	if (tech.AddShader(GL_FRAGMENT_SHADER, "ps.glsl") != true) assert(0);
	if (tech.Finalize() != true) assert(0);
	tech.Enable();

	SDL_ShowCursor(0);
	matId = tech.GetUniformLocation("MVP");
	GLint texID = tech.GetUniformLocation("gColorMap");
	glUniformMatrix4fv(matId, 1, GL_FALSE, &mvp[0][0]);
	glUniform1i(texID, 0);

	uint32_t boneId = tech.GetUniformLocation("bones");

	SWORD::ResourceGroup rg;
	std::string module_id = rg.loadModule("war_fbx.FBX");
	SWORD::ModulePtr module = rg.get<rg.MODULE>(module_id);

	uint32_t VAO[10];
	glGenVertexArrays(module->sub_meshs.size(), VAO);
	for (size_t i = 0; i < module->sub_meshs.size(); i++) {
		SWORD::MeshPtr mesh = rg.get<rg.MESH>(module->sub_meshs[i]);
		
		glBindVertexArray(VAO[i]);

		uint32_t pos, tex, index, bone, weight;
		glGenBuffers(1, &pos);
		glGenBuffers(1, &tex);
		glGenBuffers(1, &index);
		glGenBuffers(1, &bone);
		glGenBuffers(1, &weight);

		glBindBuffer(GL_ARRAY_BUFFER,pos);
		glBufferData(GL_ARRAY_BUFFER, mesh->vertex.size() * sizeof(mesh->vertex[0]),
					 mesh->vertex.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, tex);
		glBufferData(GL_ARRAY_BUFFER, mesh->texCoord.size() * sizeof(mesh->texCoord[0]),
					 mesh->texCoord.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->index.size() * sizeof(mesh->index[0]),
					 mesh->index.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, bone);
		glBufferData(GL_ARRAY_BUFFER, mesh->joint_id.size() * sizeof(mesh->joint_id[0]),
					 mesh->joint_id.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_INT, 0, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, weight);
		glBufferData(GL_ARRAY_BUFFER, mesh->joint_weight.size() * sizeof(mesh->joint_weight[0]),
					 mesh->joint_weight.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);

	}

	SWORD::MeshPtr mesh0 = rg.get<rg.MESH>(module->sub_meshs[0]);
	SWORD::SkeletonPtr ske = rg.get<rg.SKELETON>(mesh0->skeleton_id);


	
	bool quit = false;
	SDL_Event evt;
	glEnable(GL_DEPTH_TEST);
	Timer time;
	time.begin();

	glUniform1i(texID, 0);
	std::vector<Matrix4f> tran;
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
			default:break;
			}
		}
		
		update(mCamera, time.sinceLastTick());
		ske->updateAnimation(time.sinceBegin()*0.001f);
		glUniformMatrix4fv(boneId, ske->getAnimationMatrix().size(),
						   GL_FALSE,
						   &ske->getAnimationMatrix().data()[0][0][0]);
		time.tick();
		assert(glGetError() == 0);

		for (size_t i = 0; i < module->sub_meshs.size(); i++) {
			glBindVertexArray(VAO[i]);

			SWORD::MeshPtr mesh = rg.get<rg.MESH>(module->sub_meshs[i]);
			SWORD::MaterialPtr material = rg.get<rg.MATERIAL>(mesh->material_id);
			SWORD::TexturePtr tex = rg.get<rg.TEXTURE>(material->tex_diff);
			tex->bindToActiveUnit(0);
			glDrawElements(GL_TRIANGLES, mesh->index.size(), GL_UNSIGNED_INT, 0);
		}

		root.swapBuffer();
	}

	glBindVertexArray(0);

	return 0;
}


