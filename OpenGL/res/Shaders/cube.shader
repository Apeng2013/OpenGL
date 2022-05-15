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

uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPos;
uniform sampler2D u_Texture;


void main()
{
	color = texture(u_Texture, v_TexCoord) * vec4(0.3, 0.6, 0.5, 1.0);
	color.w = 1;

	float ambient = 0.1;

	vec3 light_dir = normalize(u_LightPosition - v_WorldPos);
	float diffuse = 0.5 * max(0, dot(light_dir, v_Normal));

	vec3 half_vector = normalize(light_dir + u_CameraPos - v_WorldPos);
	float specular = 0.5 * pow(max(0, dot(half_vector, v_Normal)), 32);
	if (dot(light_dir, v_Normal) < 0.0)
		specular = 0.0;

	color = vec4((ambient + diffuse + specular) * u_LightColor, 1.0) * color;
}