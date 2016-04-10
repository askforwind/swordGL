/*#include"Application.h"*/
//#include<string.h>
//int main() {
//Application a;
//a.run();
//return 0;
//}
//
#include<GL/glew.h>
#include"Root.h"
#include"shader.hpp"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"Texture.h"
#include<iostream>
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,

    -1.0, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
};
/*
 *       /2------/|6
 *     3/|------/7|
 *      ||      | |
 *      |/0-----|-/4
 *      +1------+/5
 */

static const GLfloat g_vertex_color_data[] = {
    0.0, 1.0,    0.0, 0.0,
    1.0, 0.0,    1.0, 1.0,

    1.0, 1.0,    0.0, 1.0,
    0.0, 0.0,    1.0, 0.0,

    0.0, 1.0,    0.0, 0.0,
    1.0, 0.0,    1.0, 1.0,

    1.0, 1.0,    0.0, 1.0,
    0.0, 0.0,    1.0, 0.0,

    0.0, 1.0,    0.0, 0.0,
    1.0, 0.0,    1.0, 1.0,

    0.0, 1.0,    0.0, 0.0,
    1.0, 0.0,    1.0, 1.0,
};
unsigned short g_idx[] = { 0, 1, 2,   0, 2, 3,
                           4, 5, 6,   4, 6, 7,
                           8, 9, 10,  8, 10, 11,
                           12, 13, 14,   12, 14, 15,
                           16, 17, 18,    16, 18, 19,
                           20, 21, 22,   20, 22, 23,
                         };

int main(int argc,char*argv[]) {
    SWORD::Root root;
    uint32_t proId = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
    int32_t matId = glGetUniformLocation(proId, "MVP");
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
                         glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
                         glm::vec3(0, 0, 0), // and looks at the origin
                         glm::vec3(0, 1, 0) // Head is up (set to 0,-1,0 to look upside-down)
                     );
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = projection * view * model;
    glUseProgram(proId);
    glUniformMatrix4fv(matId, 1, GL_FALSE, &mvp[0][0]);

    //uint32_t texID = loadTexture();

    SWORD::Texture mTex;
    mTex.load("t.bmp");

    int32_t  tex0 = glGetUniformLocation(proId, "tex0");
    uint32_t elevao;
    glGenVertexArrays(1, &elevao);
    glBindVertexArray(elevao);
    uint32_t idxbuff;
    glGenBuffers(1, &idxbuff);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxbuff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_idx), g_idx, GL_STATIC_DRAW);
    uint32_t verbuff;
    glGenBuffers(1, &verbuff);
    glBindBuffer(GL_ARRAY_BUFFER, verbuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    uint32_t coloUV;
    glGenBuffers(1, &coloUV);
    glBindBuffer(GL_ARRAY_BUFFER, coloUV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_color_data), g_vertex_color_data, GL_STATIC_DRAW);


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

        mTex.bindToActiveUnit(1);
        glUniform1i(tex0, 1);
        glDrawElements(GL_TRIANGLES,
                       36, GL_UNSIGNED_SHORT, 0);
        root.swapBuffer();
    }
    mTex.unload();
    glBindVertexArray(0);
    glDeleteProgram(proId);
    glDeleteBuffers(1, &verbuff);
    glDeleteBuffers(1, &idxbuff);
    glDeleteVertexArrays(1, &elevao);
    return 0;
}
























































































































