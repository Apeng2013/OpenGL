# vertex shader

#version 330 core
layout(location = 0) in vec4 aPos;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * aPos;
}


# fragment shader

#version 330 core

out vec4 color;

uniform vec3 u_LightColor;

void main()
{
	color = vec4(u_LightColor, 1.0);
}