# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 aNormal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_WorldPos;

uniform mat4 u_MVP;
uniform mat4 u_Model;

void main()
{
	v_TexCoord = texCoord;
	v_Normal = normalize(mat3(transpose(inverse(u_Model))) * aNormal);
	v_WorldPos = vec3(u_Model * vec4(aPos, 1.0));
	gl_Position = u_MVP * vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_WorldPos;

out vec4 color;

uniform vec3 u_CameraPos;
uniform sampler2D u_Texture;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	int shinness;
};

uniform Material material;

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

void main()
{
	vec3 ambient = light.ambient * material.ambient;

	vec3 light_dir = normalize(light.position - v_WorldPos);
	float diff = 0.5 * max(0, dot(light_dir, v_Normal));
	vec3 diffuse = diff * light.diffuse * material.diffuse;

	vec3 specular = vec3(0.0, 0.0, 0.0);
	if (dot(light_dir, v_Normal) > 0.0)
	{
		vec3 half_vector = normalize(light_dir + u_CameraPos - v_WorldPos);
		float spec = pow(max(0, dot(half_vector, v_Normal)), material.shinness);
		specular = spec * light.specular * material.specular;
	}

	color = vec4(ambient + diffuse + specular, 1.0);
}