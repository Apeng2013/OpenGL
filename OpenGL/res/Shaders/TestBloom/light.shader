# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 bright;

uniform vec3 uColor;

void main()
{
	color = vec4(uColor, 1.0);
	float brightness = dot(uColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0)
	{
		bright = color;
	}
}