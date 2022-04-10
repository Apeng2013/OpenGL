# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;

void main()
{
	v_texCoord = texCoord;
	gl_Position = vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

out vec4 color;

in vec2 v_texCoord;
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_texCoord);
}