# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 v_normal;
out vec3 v_worldPos;
out vec2 v_texCoord;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	v_texCoord = texCoord;
	v_normal = normalize(mat3(transpose(inverse(uModel))) * normal);
	v_worldPos = vec3(uModel * vec4(aPos, 1.0));
	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

out vec4 color;

in vec3 v_normal;
in vec3 v_worldPos;
in vec2 v_texCoord;

uniform vec3 uCameraPos;
uniform int uShinness;

uniform sampler2D utexture_diffuse1;
//uniform sampler2D utexture_diffuse2;
//uniform sampler2D utexture_diffuse3;
//uniform sampler2D utexture_diffuse4;
//uniform sampler2D utexture_diffuse5;

uniform sampler2D utexture_specular1;
//uniform sampler2D utexture_specular2;
//uniform sampler2D utexture_specular3;
//uniform sampler2D utexture_specular4;
//uniform sampler2D utexture_specular5;

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


vec3 CalculateDirectionLight(DirectionLight light, vec3 diffuse_color, vec3 specular_color)
{
	vec3 light_dir = normalize(-light.direction);

	vec3 ambient = light.ambient * diffuse_color;

	float diff = max(0, dot(light_dir, v_normal));
	vec3 diffuse = diff * light.diffuse * diffuse_color;

	vec3 reflect_dir = reflect(-light_dir, v_normal);
	vec3 view_dir = normalize(uCameraPos - v_worldPos);
	float spec = pow(max(0, dot(reflect_dir, view_dir)), uShinness);
	vec3 specular = spec * light.specular * specular_color;

	return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 diffuse_color, vec3 specular_color)
{
	vec3 light_dir = normalize(light.position - v_worldPos);

	vec3 ambient = light.ambient * diffuse_color;

	float distance = length(light.position - v_worldPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	float diff = max(0, dot(light_dir, v_normal));
	vec3 diffuse = attenuation * diff * light.diffuse * diffuse_color;

	vec3 reflect_dir = reflect(-light_dir, v_normal);
	vec3 view_dir = normalize(uCameraPos - v_worldPos);
	float spec = pow(max(0, dot(reflect_dir, view_dir)), uShinness);
	vec3 specular = attenuation * spec * light.specular * specular_color;

	return ambient + diffuse + specular;
}

vec3 CalculateSpotLight(SpotLight light, vec3 diffuse_color, vec3 specular_color)
{
	vec3 light_dir = normalize(light.position - v_worldPos);

	vec3 ambient = light.ambient * diffuse_color;

	float theta = dot(normalize(light.direction), -light_dir);
	float epsilon = light.cutoff - light.outercutoff;
	float intensity = clamp((theta - light.outercutoff) / epsilon, 0.0, 1.0);

	float diff = max(0, dot(light_dir, v_normal));
	vec3 diffuse = intensity * diff * light.diffuse * diffuse_color;

	vec3 reflect_dir = reflect(-light_dir, v_normal);
	vec3 view_dir = normalize(uCameraPos - v_worldPos);
	float spec = pow(max(0, dot(reflect_dir, view_dir)), uShinness);
	vec3 specular = intensity * spec * light.specular * specular_color;

	return ambient + diffuse + specular;
}


void main()
{
	vec3 tex_color = vec3(texture(utexture_diffuse1, v_texCoord));
	//vec3 spec_color = vec3(texture(utexture_specular1, v_texCoord));

	//vec3 o_color = CalculateDirectionLight(uDirectionLight, tex_color, spec_color);

	//o_color += CalculatePointLight(uPointLight, tex_color, spec_color);

	//o_color += CalculateSpotLight(uSpotLight, tex_color, spec_color);

	color = vec4(tex_color, 1.0);
}