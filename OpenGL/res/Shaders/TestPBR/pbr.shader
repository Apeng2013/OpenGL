# vertex shader

#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
	vec2 texCoord;
	vec3 fragPos;
	vec3 normal;
} vs_out;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	vs_out.texCoord = texCoord;
	vs_out.fragPos = vec3(uModel * vec4(pos, 1.0));
	vs_out.normal = mat3(transpose(inverse(uModel))) * normal;
	gl_Position = uProj * uView * uModel * vec4(pos, 1.0);
}

# fragment shader

#version 330 core

in VS_OUT
{
	vec2 texCoord;
	vec3 fragPos;
	vec3 normal;
} fs_in;

out vec4 FragColor;


uniform vec3 uCameraPos;
uniform vec3 uAlbedo;
uniform float uMetallic;
uniform float uRoughness;
uniform float uAo;

struct PointLight
{
	vec3 LightPos;
	vec3 LightColor;
};

uniform PointLight[2] uPointLights;

const float PI = 3.14159265359;

vec3 FresnelSchlick(vec3 f0, float cosTheta)
{
	return f0 + (1 - f0) * pow(1 - cosTheta, 5.0);
}


float DistributionGGX(vec3 normal, vec3 h, float roughness)
{
	float a = roughness * roughness;
	float b = dot(normal, h) * dot(normal, h) * (a - 1) + 1;

	return a / (PI * b * b);
}

float GeometrySchlickGGX(float NdotV, float k)
{
	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, k);
	float ggx2 = GeometrySchlickGGX(NdotL, k);

	return ggx1 * ggx2;
}

void main()
{
	vec3 normal = normalize(fs_in.normal);
	vec3 view_dir = normalize(uCameraPos - fs_in.fragPos);

	vec3 Lo = vec3(0.0);

	for (int i = 0; i < 2; i++)
	{
		vec3 light_dir = normalize(uPointLights[i].LightPos - fs_in.fragPos);
		vec3 half_dir = normalize(view_dir + light_dir);
		
		float distance = length(uPointLights[i].LightPos - fs_in.fragPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = uPointLights[i].LightColor * attenuation;

		vec3 f0 = vec3(0.04);
		f0 = mix(f0, uAlbedo, uMetallic);

		vec3 F = FresnelSchlick(f0, max(dot(normal, view_dir), 0));

		float NDF = DistributionGGX(normal, half_dir, uRoughness);

		float G = GeometrySmith(normal, view_dir, light_dir, uRoughness);

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - uMetallic;

		vec3 nominator = NDF * G * F;
		float denominator = 4.0 * max(dot(normal, view_dir), 0.0) * max(dot(normal, light_dir), 0.0) + 0.001;
		vec3 specular = nominator / denominator;

		// add to outgoing radiance Lo
		float NdotL = max(dot(normal, light_dir), 0.0);
		Lo += (kD * uAlbedo / PI + specular) * radiance * NdotL;

	}

	vec3 ambient = vec3(0.03) * uAlbedo * uAo;
	vec3 color = ambient + Lo;

	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(color, 1.0);
}

