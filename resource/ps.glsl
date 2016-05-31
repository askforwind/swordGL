#version 330 core


in vec2 v_uv;

// the texture
uniform sampler2D gColorMap;

uniform vec3 eye;
uniform float shininess;
uniform vec3 spec;
uniform sampler2D colorMap0;
uniform vec3 diff;

layout(std140) uniform light{
	vec3 direction;
	vec3 color;
};

void main() {
	
	gl_FragColor =vec4( texture(gColorMap, uv).rgb,1.0);
//	gl_FragColor=vec4(1.0,1.0,1.0,1.0);
}