# vertex shader

#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
	vec2 v_texCoord;
	vec3 v_WorldPos;
	mat3 TBN;
} vs_out;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;
uniform mat4 uTangentSpace;


void main()
{
	vs_out.v_texCoord = texCoord;
	vs_out.v_WorldPos = vec3(uModel * vec4(pos, 1.0));
	vs_out.TBN = mat3(uModel * uTangentSpace);
	gl_Position = uProj * uView * uModel * vec4(pos, 1.0);
}


# fragment shader

#version 330 core

out vec4 color;

in VS_OUT
{
	vec2 v_texCoord;
	vec3 v_WorldPos;
	mat3 TBN;
} fs_in;

struct DirectionLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform sampler2D uTexture;
uniform sampler2D uNormalTex;
uniform int uShinness;
uniform DirectionLight uDirectionLight;
uniform vec3 uCameraPos;


void main()
{
	vec3 view_dir = normalize(uCameraPos - fs_in.v_WorldPos);

	vec3 normal = vec3(texture(uNormalTex, fs_in.v_texCoord));
	normal = fs_in.TBN * normal;

	vec3 tex_color = vec3(texture(uTexture, fs_in.v_texCoord));

	vec3 ambient = uDirectionLight.ambient * tex_color;

	vec3 light_dir = normalize(-uDirectionLight.direction);
	float diff = max(0, dot(normal, light_dir));
	vec3 diffuse = diff * uDirectionLight.diffuse * tex_color;

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(0, dot(reflect_dir, view_dir)), uShinness);
	vec3 specular = spec * uDirectionLight.specular * tex_color;

	color = vec4(ambient + diffuse + specular, 1.0);
}