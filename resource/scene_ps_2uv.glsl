#version 330 core

in vec2 v_uv0;
in vec3 v_normal;
in vec3 v_pos;

uniform vec3 eye;
uniform float shininess;
uniform vec3 spec;
uniform sampler2D colorMap0;
uniform vec3 diff;

layout(std140) uniform light{
	vec3 direction;
	vec3 color;
};
vec3 calcDiffuseLight(vec3 dir,vec3 color,vec3 diff,vec3 diffMapColor ){
	vec3 diffuse = vec3(0.0,0.0,0.0);
	float diffuseFactor = dot(v_normal,-dir);
	
	
	 diffuse = max(0.0f,diffuseFactor) * color * diffMapColor * diff;
	
	 return diffuse;
}

void main() {
	
	vec3 diffuseMapColor = 
		texture(colorMap0, v_uv0).rgb ;
	
	vec3 ambient = vec3(0.4,0.4,0.4) * diffuseMapColor;
	
	vec3 ld=normalize(light.direction);
	
	vec3 diffuse = calcDiffuseLight(ld,light.color,diff,diffuseMapColor);
	
	vec3 specular = vec3(0.0,0.0,0.0);
	vec3 eyeToVertex = normalize(eye - v_pos);
	vec3 lightReflect = normalize(reflect(ld, v_normal));
	float specularFactor = dot(eyeToVertex,lightReflect);
	
	if(specularFactor > 0)
	specular = pow(specularFactor,shininess)* light.color*spec ;
	
	
	gl_FragColor = vec4(ambient +diffuse +specular  ,1.0);
}
