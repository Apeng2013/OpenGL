# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 texCoord;

layout(location = 2) in mat4 model;

out vec2 v_texCoord;
uniform mat4 uProj;
uniform mat4 uView;

void main()
{
	v_texCoord = texCoord;
	gl_Position = uProj * uView * model * vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

out vec4 color;

in vec2 v_texCoord;
uniform sampler2D uTexture;

void main()
{
	color = texture(uTexture, v_texCoord);
}