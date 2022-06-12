# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_worldPos;


uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	v_texCoord = texCoord;
	v_normal = normalize(mat3(transpose(inverse(uModel))) * normal);
	v_worldPos = vec3(uModel * vec4(aPos, 1.0));
	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

out vec4 color;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_worldPos;

uniform samplerCube uTexture;
uniform vec3 uCameraPos;

vec4 GetReflection()
{
	vec3 in_dir = normalize(v_worldPos - uCameraPos);
	vec3 dir = reflect(in_dir, v_normal);
	return texture(uTexture, dir);
}

vec4 GetRefraction()
{
	float ratio = 1.00 / 1.52;
	vec3 in_dir = normalize(v_worldPos - uCameraPos);
	vec3 dir = refract(in_dir, v_normal, ratio);
	return vec4(texture(uTexture, dir).rgb, 1.0);
}


void main()
{
	color = GetRefraction();
}