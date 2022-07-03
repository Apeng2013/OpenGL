# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} vs_out;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	vs_out.texCoord = texCoord;
	vs_out.normal = normalize(mat3(transpose(inverse(uModel))) * aNormal);
	vs_out.worldPos = vec3(uModel * vec4(aPos, 1.0));

	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}


# fragment shader

#version 330 core
layout(location = 0) out vec4 color;
layout(location = 1) out vec4 bright;

in VS_OUT
{
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} fs_in;


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

uniform PointLight uPointLight1;
uniform PointLight uPointLight2;

vec3 CalculatePointLight(PointLight light, vec3 view_dir)
{
	vec3 tex_color = vec3(texture(uTexture, fs_in.texCoord));
	vec3 spec_color = vec3(texture(uSpecTex, fs_in.texCoord));

	vec3 light_dir = normalize(light.position - fs_in.worldPos);
	vec3 ambient = light.ambient * tex_color;

	float distance = length(light.position - fs_in.worldPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	float diff = max(0, dot(light_dir, fs_in.normal));
	vec3 diffuse = attenuation * diff * light.diffuse * tex_color;

	vec3 reflect_dir = reflect(-light_dir, fs_in.normal);
	float spec = pow(max(0, dot(reflect_dir, view_dir)), uShinness);
	vec3 specular = attenuation * spec * light.specular * spec_color;

	return ambient + diffuse + specular;
}


void main()
{

	vec3 view_dir = normalize(uCameraPos - fs_in.worldPos);

	vec3 point_light = CalculatePointLight(uPointLight1, view_dir);
	point_light += CalculatePointLight(uPointLight2, view_dir);

	color = vec4(point_light, 1.0);

	float brightness = dot(point_light.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0)
	{
		bright = color;
	}

}