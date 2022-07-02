# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
	vec2 texCoord;
} vs_out;

void main()
{
	vs_out.texCoord = texCoord;
	gl_Position = vec4(aPos, 1.0);
}

# fragment shader

#version 330 core

out vec4 color;

uniform sampler2D uTexture;

in VS_OUT
{
	vec2 texCoord;
} fs_in;

void main()
{
	vec3 hdr_color = texture(uTexture, fs_in.texCoord).rgb;

	color = vec4(hdr_color / (hdr_color + vec3(1.0)), 1.0);
	//color = vec4(hdr_color, 1.0);
}
