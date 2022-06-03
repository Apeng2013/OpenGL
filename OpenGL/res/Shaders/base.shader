# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 noraml;
layout(location = 2) in vec2 texCoord;

out vec2 v_texCoord;
uniform mat4 u_MVP;

void main()
{
	v_texCoord = texCoord;
	gl_Position = u_MVP * vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

out vec4 color;

in vec2 v_texCoord;

void main()
{
	color = vec4(1.0, 1.0, 1.0, 1.0);
}