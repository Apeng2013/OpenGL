# vertex shader
# version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 uProj;
uniform mat4 uView;

out VS_OUT
{
	vec3 worldPos;
} vs_out;

void main()
{
	vs_out.worldPos = pos;
	vec4 p = uProj * mat4(mat3(uView)) * vec4(pos, 1.0);
	gl_Position = p.xyww;
}

# fragment shader

# version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 worldPos;
}fs_in;

uniform samplerCube uTexture;

void main()
{
	vec3 color = texture(uTexture, fs_in.worldPos).rgb;
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));
	FragColor = vec4(color, 1.0);
}




