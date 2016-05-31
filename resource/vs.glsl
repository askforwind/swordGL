#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex;
layout(location = 2) in ivec4 boneIDs;
layout(location = 3) in vec4 weights;
layout(location = 4) in vec3 normal;

uniform mat4 VP;
uniform mat4 Model;
uniform mat4 bones[100];

out vec2 v_uv0;
out vec3 v_normal;
out vec3 v_pos;

void main() {

	mat4 boneTransform = bones[boneIDs[0]] * weights[0];
	boneTransform += bones[boneIDs[1]] * weights[1];
	boneTransform += bones[boneIDs[2]] * weights[2];
	boneTransform += bones[boneIDs[3]] * weights[3];

	v_pos = (Model * boneTransform * vec4(position, 1.0)).xyz;
	gl_Position = VP * vec4( v_pos,1.0) ;
	
	v_uv0 = tex;
	v_normal = (Model * boneTransform * vec4(normal,0.0)).xyz;
	
}