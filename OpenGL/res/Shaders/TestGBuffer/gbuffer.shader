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

in VS_OUT
{
	vec2 texCoord;
} fs_in;

out vec4 color;

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform PointLight uPointLight;

uniform sampler2D uPositionTex;
uniform sampler2D uNormalTex;
uniform sampler2D uColorTex;

uniform vec3 uCameraPos;
uniform float uShinness;

void main()
{
	vec3 position = texture(uPositionTex, fs_in.texCoord).rgb;
	vec3 normal = texture(uNormalTex, fs_in.texCoord).rgb;
	vec3 frag_color = texture(uColorTex, fs_in.texCoord).rgb;

	vec3 view_dir = normalize(uCameraPos - position);
	vec3 light_dir = normalize(uPointLight.position - position);

	vec3 ambient = uPointLight.ambient * frag_color;

	float distance = length(uPointLight.position - position);
	float attenuation = 1.0 / (uPointLight.constant + uPointLight.linear * distance + uPointLight.quadratic * distance * distance);

	float diff = max(0, dot(light_dir, normal));
	vec3 diffuse = attenuation * diff * uPointLight.diffuse * frag_color;

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(0, dot(reflect_dir, view_dir)), uShinness);
	vec3 specular = attenuation * spec * uPointLight.specular;

	color = vec4(ambient + diffuse + specular, 1.0);
}