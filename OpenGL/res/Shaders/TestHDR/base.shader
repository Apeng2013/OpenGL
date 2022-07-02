# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_WorldPos;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	v_TexCoord = texCoord;
	v_Normal = normalize(mat3(transpose(inverse(uModel))) * aNormal);
	v_WorldPos = vec3(uModel * vec4(aPos, 1.0));

	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_WorldPos;

out vec4 color;

uniform vec3 uCameraPos;
uniform sampler2D uTexture;
uniform sampler2D uSpecTex;
uniform int uShinness;

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

vec3 CalculatePointLight(PointLight light, vec3 view_dir)
{
	vec3 tex_color = vec3(texture(uTexture, v_TexCoord));
	vec3 spec_color = vec3(texture(uSpecTex, v_TexCoord));

	vec3 light_dir = normalize(light.position - v_WorldPos);
	vec3 ambient = light.ambient * tex_color;

	float distance = length(light.position - v_WorldPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	float diff = max(0, dot(light_dir, v_Normal));
	vec3 diffuse = attenuation * diff * light.diffuse * tex_color;

	vec3 reflect_dir = reflect(-light_dir, v_Normal);
	float spec = pow(max(0, dot(reflect_dir, view_dir)), uShinness);
	vec3 specular = attenuation * spec * light.specular * spec_color;

	return ambient + diffuse + specular;
}


void main()
{

	vec3 view_dir = normalize(uCameraPos - v_WorldPos);

	color = vec4(CalculatePointLight(uPointLight, view_dir), 1.0);

}