# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
	vec2 v_texCoord;
} vs_out;

layout(std140) uniform Matrices
{
	mat4 uProj;
	mat4 uView;
};

uniform mat4 uModel;

void main()
{
	vs_out.v_texCoord = texCoord;
	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

out vec4 color;

in VS_OUT
{
	vec2 v_texCoord;
} fs_in;

uniform sampler2D uTexture;

void main()
{
	color = texture(uTexture, fs_in.v_texCoord);
}