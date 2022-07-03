# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} vs_out;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	vs_out.texCoord = texCoord;
	vs_out.normal = normalize(mat3(transpose(inverse(uModel))) * aNormal);
	vs_out.worldPos = vec3(uModel * vec4(aPos, 1.0));

	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}


# fragment shader

#version 330 core
layout(location = 0) out vec3 position;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec4 color;

in VS_OUT
{
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} fs_in;

uniform sampler2D uTexture;

void main()
{
	position = fs_in.worldPos;
	normal = fs_in.normal;
	color = texture(uTexture, fs_in.texCoord);
}