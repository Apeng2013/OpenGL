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
uniform samplerCube uIrradianceMap;
uniform samplerCube uPreFilterMap;
uniform sampler2D uBRDFMap;

const float PI = 3.14159265359;

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	vec3 normal = normalize(fs_in.normal);
	vec3 view_dir = normalize(uCameraPos - fs_in.fragPos);

	vec3 f0 = vec3(0.04);
	f0 = mix(f0, uAlbedo, uMetallic);

	vec3 kS = fresnelSchlickRoughness(max(dot(normal, view_dir), 0.0), f0, uRoughness);
	vec3 kD = 1.0 - kS;
	vec3 irradiance = texture(uIrradianceMap, normal).rgb;

	vec3 diffuse = irradiance * uAlbedo;

	vec3 ambient = (kD * diffuse) * uAo;

	ambient = ambient / (ambient + vec3(1.0));
	ambient = pow(ambient, vec3(1.0 / 2.2));

	vec3 R = reflect(-view_dir, fs_in.normal);

	const float MAX_REFLECTION_LOD = 4.0;
	vec3 prefilteredColor = textureLod(uPreFilterMap, R, uRoughness * MAX_REFLECTION_LOD).rgb;

	vec2 envBRDF = texture(uBRDFMap, vec2(max(dot(normal, view_dir), 0.0), uRoughness)).rg;
	vec3 specular = prefilteredColor * (kS * envBRDF.x + envBRDF.y);

	FragColor = vec4(ambient + specular, 1.0);
}

