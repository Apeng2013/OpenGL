# vertex shader

#version 330 core
layout(location = 0) in vec4 aPos;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * aPos;
}


# fragment shader

#version 330 core

out vec4 color;

uniform vec3 uLightColor;

void main()
{
	color = vec4(uLightColor, 1.0);
}