# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT{
	vec2 v_texCoord;
	vec4 v_fragLightPos;
} vs_out;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

uniform mat4 uLightProj;
uniform mat4 uLightView;

void main()
{
	vs_out.v_texCoord = texCoord;
	vs_out.v_fragLightPos = uLightProj * uLightView * uModel * vec4(aPos, 1.0);

	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

out vec4 color;

in VS_OUT{
	vec2 v_texCoord;
	vec4 v_fragLightPos;
} fs_in;

uniform sampler2D uTexture;
uniform sampler2D uShadowTex;

float CalculateShadow(vec4 fragLightPos)
{
	vec3 projCoords = fragLightPos.xyz / fragLightPos.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(uShadowTex, projCoords.xy).r;
	float currentDepth = projCoords.z;
	if (currentDepth > 1.0)
		return 0.0;
	float shadow = currentDepth - 0.005 > closestDepth ? 1.0 : 0.0;
	return shadow;
}

void main()
{
	float shadow = CalculateShadow(fs_in.v_fragLightPos);
	color = vec4(vec3(texture(uTexture, fs_in.v_texCoord) * (1.0 - shadow)), 1.0);
}