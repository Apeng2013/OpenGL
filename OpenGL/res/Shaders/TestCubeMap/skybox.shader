# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 v_texCoord;
uniform mat4 uProj;
uniform mat4 uView;

void main()
{
	v_texCoord = aPos;
	vec4 pos = uProj * uView * vec4(aPos, 1.0);
	gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
}


# fragment shader

#version 330 core

out vec4 color;

in vec3 v_texCoord;
uniform samplerCube uTexture;

void main()
{
	color = texture(uTexture, v_texCoord);
}