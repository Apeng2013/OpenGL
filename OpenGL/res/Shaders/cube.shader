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
	vec3 tex_color = vec3(texture(uDiffuseTexture, v_TexCoord));

	vec3 ambient = light.ambient * tex_color;

	vec3 light_dir = normalize(light.position - v_WorldPos);
	float diff = max(0, dot(light_dir, v_Normal));
	vec3 diffuse = diff * light.diffuse * tex_color;

	vec3 specular = vec3(0.0, 0.0, 0.0);
	if (dot(light_dir, v_Normal) > 0.0)
	{
		vec3 half_vector = normalize(light_dir + uCameraPos - v_WorldPos);
		float spec = pow(max(0, dot(half_vector, v_Normal)), material.shinness);
		specular = spec * light.specular * texture(uSpecularTexture, v_TexCoord);
	}

	color = vec4(ambient + diffuse + specular, 1.0);
}