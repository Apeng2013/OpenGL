# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_texCoord;
uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	v_texCoord = texCoord;
	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
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