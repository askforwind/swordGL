#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord0;
layout(location = 2) in vec3 a_normal;

uniform mat4 VP;

uniform mat4 Model;

out vec2 v_uv0;
out vec3 v_normal;
out vec3 v_pos;

void main() {

	v_uv0 = a_texCoord0;
	v_pos = (Model * vec4(a_position, 1.0)).xyz ;
	v_normal = (Model * vec4 (a_normal,0.0)).xyz;

	gl_Position =  VP * vec4(v_pos, 1.0);
}