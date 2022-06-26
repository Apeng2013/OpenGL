# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT{
	vec2 v_texCoord;
	vec4 v_fragWorldPos;
} vs_out;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	vs_out.v_texCoord = texCoord;
	vs_out.v_fragWorldPos = uModel * vec4(aPos, 1.0);
	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

out vec4 color;

in VS_OUT{
	vec2 v_texCoord;
	vec4 v_fragWorldPos;
} fs_in;

uniform sampler2D uTexture;
uniform samplerCube uShadowTex;

uniform vec3 uLightPos;
uniform float uFarPlane;

float CalculateShadow(vec4 fragWorldPos)
{
	vec3 dir = fragWorldPos.xyz - uLightPos;

	float closestDepth = texture(uShadowTex, dir).r * uFarPlane;
	float currentDepth = length(dir);

	float shadow = currentDepth - 0.005 > closestDepth ? 1.0 : 0.0;
	return shadow;
}

void main()
{
	float shadow = CalculateShadow(fs_in.v_fragWorldPos);
	color = vec4(vec3(texture(uTexture, fs_in.v_texCoord) * (1.0 - shadow)), 1.0);
}