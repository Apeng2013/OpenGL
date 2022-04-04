# vertext shader

#version 330 core
layout(location = 0) in vec3 aPos;
void main()
{
	gl_Position = vec4(aPos, 1.0);
}


# fragment shader

#version 330 core
out vec4 color;

void main()
{
	color = vec4(1.0, 0.5, 0.2, 1.0);
}