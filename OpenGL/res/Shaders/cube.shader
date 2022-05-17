# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_WorldPos;

uniform mat4 uMVP;
uniform mat4 uModel;

void main()
{
	v_TexCoord = texCoord;
	v_Normal = normalize(mat3(transpose(inverse(uModel))) * aNormal);
	v_WorldPos = vec3(uModel * vec4(aPos, 1.0));
	gl_Position = uMVP * vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_WorldPos;

out vec4 color;

uniform vec3 uCameraPos;
uniform sampler2D uDiffuseTexture;
uniform sampler2D uSpecularTexture;

struct Material
{
	int shinness;
};

uniform Material material;

struct DirectionLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirectionLight uDirectionLight;

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

struct SpotLight
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float cutoff;
	float outercutoff;
};

uniform SpotLight uSpotLight;


void main()
{
	vec3 tex_color = vec3(texture(uDiffuseTexture, v_TexCoord));

	vec3 ambient = uDirectionLight.ambient * tex_color;

	vec3 light_dir = normalize(-uDirectionLight.direction);
	float diff = max(0, dot(light_dir, v_Normal));
	vec3 diffuse = diff * uDirectionLight.diffuse * tex_color;

	vec3 specular = vec3(0.0, 0.0, 0.0);
	if (dot(light_dir, v_Normal) > 0.0)
	{
		vec3 half_vector = normalize(light_dir + uCameraPos - v_WorldPos);
		float spec = pow(max(0, dot(half_vector, v_Normal)), material.shinness);
		specular = spec * uDirectionLight.specular * texture(uSpecularTexture, v_TexCoord);
	}

	ambient = uPointLight.ambient * tex_color;
	light_dir = normalize(uPointLight.position - v_WorldPos);
	diff = max(0, dot(light_dir, v_Normal));
	diffuse = diff * uPointLight.diffuse * tex_color;
	specular = vec3(0, 0, 0);
	if (dot(light_dir, v_Normal) > 0.0)
	{
		vec3 half_vector = normalize(light_dir + uCameraPos - v_WorldPos);
		float spec = pow(max(0, dot(half_vector, v_Normal)), material.shinness);
		specular = spec * uPointLight.specular * texture(uSpecularTexture, v_TexCoord);
	}
	float distance = length(uPointLight.position - v_WorldPos);
	float attenuation = 1.0 / (uPointLight.constant + uPointLight.linear * distance + uPointLight.quadratic * distance * distance);

	ambient = uSpotLight.ambient * tex_color;
	light_dir = normalize(uSpotLight.position - v_WorldPos);
	float theta =dot(light_dir, normalize(-uSpotLight.direction));
	float epsilon = uSpotLight.cutoff - uSpotLight.outercutoff;
	float intensity = clamp((theta - uSpotLight.outercutoff) / epsilon, 0.0, 1.0);
	diff = max(0, dot(light_dir, v_Normal));
	diffuse = intensity * diff * uSpotLight.diffuse * tex_color;
	vec3 half_vector = normalize(light_dir + uCameraPos - v_WorldPos);
	float spec = pow(max(0, dot(half_vector, v_Normal)), material.shinness);
	specular = intensity * spec * uSpotLight.specular * texture(uSpecularTexture, v_TexCoord);
	color = vec4(ambient + diffuse + specular, 1.0);

}