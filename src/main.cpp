#include<GL/glew.h>
#include"Root.h"
#include"shader.hpp"
#include"FPSCamera.h"
#include"ResourceGroup.h"
#include<iostream>
#include"SDL2\SDL_mouse.h"

int32_t matId;
SWORD::Root root;
void mouseMove(const SDL_MouseMotionEvent &arg,SWORD::FPSCamera& c) {
	float x_offset = 300 - arg.x;
	float y_offset = 400 - arg.y;
	c.moveMouse(x_offset, y_offset, 0.0001);
	glm::mat4 mvp = c.get_matrix()*glm::mat4(1.0f);
	glUniformMatrix4fv(matId, 1, GL_FALSE, &mvp[0][0]);
	SDL_WarpMouseInWindow(root.get_render_window()->get_win_handle(), 300, 400);
}

void keyPress(const SDL_KeyboardEvent &arg, SWORD::FPSCamera& c) {
	if (arg.keysym.sym == SDLK_w) {
		c.forward(0.01);
	}
	if (arg.keysym.sym == SDLK_s) {
		c.backward(0.01);
	}
	if (arg.keysym.sym == SDLK_a) {
		c.moveLeft(0.01);
	}
	if (arg.keysym.sym == SDLK_d) {
		c.moveRight(0.01);
	}
	glm::mat4 mvp = c.get_matrix()*glm::mat4(1.0f);
	glUniformMatrix4fv(matId, 1, GL_FALSE, &mvp[0][0]);
}

int main(int argc,char*argv[]) {
   
	SWORD::ResourceGroup mResource;
	mResource.initialise();
	SDL_WarpMouseInWindow(root.get_render_window()->get_win_handle(), 300, 400);
	SDL_ShowCursor(0);
	SWORD::FPSCamera mCamera;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	mCamera.set_clip_plane(0.1f, 100.0f);
	mCamera.set_aspect(4.0f / 3.0f);
	
	
	mCamera.set_position(glm::vec3(0, 0, 5));
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
	SWORD::Module* mod = mResource.loadModule("cube.blend", "cube");

	const SWORD::Mesh* mesh = mResource.get_mesh(mod->operator[](0));
	const SWORD::Mesh::Info* info = mesh->get_info();

    uint32_t elevao;
    glGenVertexArrays(1, &elevao);
    glBindVertexArray(elevao);
    uint32_t idxbuff;
    glGenBuffers(1, &idxbuff);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxbuff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t)*info->index.size(),
				 info->index.data(), GL_STATIC_DRAW);

    uint32_t verbuff;
    glGenBuffers(1, &verbuff);
    glBindBuffer(GL_ARRAY_BUFFER, verbuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*info->vertex.size(),
				 info->vertex.data(), GL_STATIC_DRAW);

    uint32_t coloUV;
    glGenBuffers(1, &coloUV);
    glBindBuffer(GL_ARRAY_BUFFER, coloUV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*info->uv.size(),
				 info->uv.data(), GL_STATIC_DRAW);


    bool quit = false;
    SDL_Event evt;
    glEnable(GL_DEPTH_TEST);
    while(!quit) {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_PollEvent(&evt);
        switch(evt.type) {
        case SDL_QUIT:
            quit = true;
            break;
		case SDL_MOUSEMOTION:
			mouseMove(evt.motion, mCamera);
		case SDL_KEYDOWN:
			keyPress(evt.key, mCamera);
        }

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, verbuff);
        glVertexAttribPointer(
            0,                  // attribute
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

		  glEnableVertexAttribArray(1);
		  glBindBuffer(GL_ARRAY_BUFFER, coloUV);
		  glVertexAttribPointer(
			  1,
			  2,
			  GL_FLOAT,
			  GL_FALSE,
			  0,
			  (void*)0
		  );

        tex->bindToActiveUnit(1);
        glUniform1i(tex0, 1);
        glDrawElements(GL_TRIANGLES,
                       info->index.size(), GL_UNSIGNED_SHORT, 0);
        root.swapBuffer();
    }
	tex->unload();
	mResource.deinitialise();
    glBindVertexArray(0);
    glDeleteProgram(proId);
    glDeleteBuffers(1, &verbuff);
    glDeleteBuffers(1, &idxbuff);
    glDeleteVertexArrays(1, &elevao);
    return 0;
}
























































































































