layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord0;
layout(location = 2) in vec3 a_normal;

uniform mat4 VP;
uniform mat4 Model;
out vec2 v_uv0;
out vec3 v_normal

void main() {

	gl_Position = MVP * vec4(position, 1.0) ;
	uv0 = texCoord0;
}