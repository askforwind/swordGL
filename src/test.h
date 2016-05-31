#pragma once
#include <GL/glew.h>
#include "Root.h"

#include <iostream>
#include <SDL2\SDL.h>
#include <SDL2\SDL_mouse.h>
#include <glm\gtx\quaternion.hpp>
#include <glm\glm.hpp>
#include "Timer.h"
#include "FPSCamera.h"
#include "ResourceGroup.h"
#include "LogManager.h"
#include "ResourceDef.h"
#include "Texture.h"
#include "Skeleton.h"
#include "TechniqueManager.h"
#include "config.h"
#include "BufferManager.h"
#include "Combiner.h"
#include "RenderQueue.h"
#include "Technique.h"
#include "EntityManager.h"
#include "Entity.h"


SWORD::Root g_root;
SWORD::TechniqueManager g_TechniqueMgr;
SWORD::BufferManager g_BufferManager;
SWORD::Combiner g_Combiner;
SWORD::ResourceGroup g_ResourceGroup;

void initCamera(SWORD::Camera* cam)
{
	cam->set_clip_plane(10.0f, 5000.0f);
	cam->set_aspect(4.0f / 3.0f);
	cam->set_position(glm::vec3(-47.9, 88.36, 27.48));
	cam->lookAt(glm::vec3(0, 0, 0));
}

SWORD::TechniquePtr initWarriorTech()
{
	SWORD::TechniquePtr tech = SWORD::TechniqueManager::instance().creatTechnique("warrior");

	if (tech->addShader(SWORD::Technique::VS, "D:/swordGL/resource/vs.glsl") != true) assert(0);
	if (tech->addShader(SWORD::Technique::PS, "D:/swordGL/resource/scene_ps_2uv.glsl") != true) assert(0);
	if (tech->finalize() != true) assert(0);
	tech->enable();
	tech->addAttributeLayout(SWORD::Attribute::POSITION, tech->get_attrib_location("position"));
	tech->addAttributeLayout(SWORD::Attribute::TEXCOORD, tech->get_attrib_location("tex"));
	tech->addAttributeLayout(SWORD::Attribute::BONE, tech->get_attrib_location("boneIDs"));
	tech->addAttributeLayout(SWORD::Attribute::WEIGHT, tech->get_attrib_location("weights"));
	tech->addAttributeLayout(SWORD::Attribute::NORMAL, tech->get_attrib_location("normal"));

	tech->addUniformLayout(SWORD::UniformTarget::BONE_TRANSFORM, tech->get_uniform_location("bones"));
	tech->addUniformLayout(SWORD::UniformTarget::VP_MATRIX, tech->get_uniform_location("VP"));
	tech->addUniformLayout(SWORD::UniformTarget::MODEL_MATRIX, tech->get_uniform_location("Model"));
	tech->addUniformLayout(SWORD::UniformTarget::DIFF_COLOR_MAP, tech->get_uniform_location("colorMap0"));
	tech->addUniformLayout(SWORD::UniformTarget::VIEW_MATRIX, tech->get_uniform_location("eye"));
	tech->addUniformLayout(SWORD::UniformTarget::DIFFUSE, tech->get_uniform_location("diff"));
	tech->addUniformLayout(SWORD::UniformTarget::SPECULAR, tech->get_uniform_location("spec"));
	tech->addUniformLayout(SWORD::UniformTarget::SHININESS, tech->get_uniform_location("shininess"));
	return tech;
}

SWORD::TechniquePtr initSceneTech()
{
	SWORD::TechniquePtr scene_tech = SWORD::TechniqueManager::instance().creatTechnique("scene");
	scene_tech->addShader(SWORD::Technique::VS, "D:/swordGL/resource/scene_vs_2uv.glsl");
	scene_tech->addShader(SWORD::Technique::PS, "D:/swordGL/resource/scene_ps_2uv.glsl");
	scene_tech->finalize();
	scene_tech->enable();
	scene_tech->addAttributeLayout(SWORD::Attribute::POSITION, scene_tech->get_attrib_location("a_position"));
	scene_tech->addAttributeLayout(SWORD::Attribute::TEXCOORD, scene_tech->get_attrib_location("a_texCoord0"));
	scene_tech->addAttributeLayout(SWORD::Attribute::NORMAL, scene_tech->get_attrib_location("a_normal"));
	scene_tech->addUniformLayout(SWORD::UniformTarget::VP_MATRIX, scene_tech->get_uniform_location("VP"));
	scene_tech->addUniformLayout(SWORD::UniformTarget::MODEL_MATRIX, scene_tech->get_uniform_location("Model"));
	scene_tech->addUniformLayout(SWORD::UniformTarget::DIFF_COLOR_MAP, scene_tech->get_uniform_location("colorMap0"));
	scene_tech->addUniformLayout(SWORD::UniformTarget::VIEW_MATRIX, scene_tech->get_uniform_location("eye"));
	scene_tech->addUniformLayout(SWORD::UniformTarget::DIFFUSE, scene_tech->get_uniform_location("diff"));
	scene_tech->addUniformLayout(SWORD::UniformTarget::SPECULAR, scene_tech->get_uniform_location("spec"));
	scene_tech->addUniformLayout(SWORD::UniformTarget::SHININESS, scene_tech->get_uniform_location("shininess"));
	return scene_tech;
}

void initDirectonLight(SWORD::TechniquePtr tech, const char* name, uint32_t bind_point)
{
	int32_t tech_prog = tech->get_prog();
	int32_t bi = glGetUniformBlockIndex(tech_prog, name);
	assert(bi != -1);
	uint32_t ubo = 0;

	int32_t num_uniform = 0;
	int32_t size = 0;
	glGetActiveUniformBlockiv(tech_prog, bi, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
	glGetActiveUniformBlockiv(tech_prog, bi, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &num_uniform);
	glUniformBlockBinding(tech_prog, bi, bind_point);
	float light[8]{ -1.0f,-1.0f,-1.0f,0.0f,0.8f,0.8f,0.8f,0.0f };
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, light, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, bind_point, ubo);
}

void bindTechToBlock(SWORD::TechniquePtr tech, const char* name,uint32_t bind_point)
{
	int32_t tech_prog = tech->get_prog();
	int32_t bi = glGetUniformBlockIndex(tech_prog, name);
	assert(bi != -1);
	glUniformBlockBinding(tech_prog, bi, bind_point);
}