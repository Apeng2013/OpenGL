# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
	vec2 texCoord;
} vs_out;


uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	vs_out.texCoord = texCoord;
	gl_Position = vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

out vec4 color;


in VS_OUT
{
	vec2 texCoord;
} fs_in;

uniform sampler2D uTexture;
uniform sampler2D uBirghtTex;

void main()
{
	vec3 screen = texture(uTexture, fs_in.texCoord).rgb;
	screen += texture(uBirghtTex, fs_in.texCoord).rgb;

	vec3 result = vec3(1.0) - exp(-screen * 0.5);
	result = pow(result, vec3(1.0 / 2.2));

	color = vec4(result, 1.0);
}