# vertex shader

#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
	vec2 v_texCoord;
	vec3 v_TangentFragPos;
	vec3 v_TangentLightPos;
	vec3 v_TangentCameraPos;
} vs_out;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;
uniform mat4 uTangentSpace;

uniform vec3 uLightPos;
uniform vec3 uCameraPos;


void main()
{
	vs_out.v_texCoord = texCoord;
	mat4 m = inverse(uModel * uTangentSpace);
	vs_out.v_TangentFragPos = vec3(inverse(uTangentSpace) * vec4(pos, 1.0));
	vs_out.v_TangentLightPos = vec3(m * vec4(uLightPos, 1.0));
	vs_out.v_TangentCameraPos = vec3(m * vec4(uCameraPos, 1.0));
	gl_Position = uProj * uView * uModel * vec4(pos, 1.0);
}


# fragment shader

#version 330 core

out vec4 color;

in VS_OUT
{
	vec2 v_texCoord;
	vec3 v_TangentFragPos;
	vec3 v_TangentLightPos;
	vec3 v_TangentCameraPos;
} fs_in;

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

uniform sampler2D uTexture;
uniform sampler2D uNormalTex;
uniform sampler2D uDepthTex;
uniform float uDepthScale;
uniform int uShinness;
uniform PointLight uPointLight;


vec2 GetParallaxMappingTexCoord(vec2 tex_coord, vec3 view_dir)
{
	float layers = 10;
	float layer_depth = 1.0 / layers;
	float current_layer_depth = 0;
	vec2 p = view_dir.xy / layers * uDepthScale;

	vec2 current_texcoord = tex_coord;
	float current_depth_map_value = texture(uDepthTex, current_texcoord).r;
	while (current_layer_depth <= current_depth_map_value)
	{
		current_texcoord -= p;
		current_depth_map_value = texture(uDepthTex, current_texcoord).r;
		current_layer_depth += layer_depth;
	}
	return current_texcoord;
}

void main()
{
	vec3 view_dir = normalize(fs_in.v_TangentCameraPos - fs_in.v_TangentFragPos);

	vec2 tex_coord = GetParallaxMappingTexCoord(fs_in.v_texCoord, view_dir);
	if (tex_coord.x > 1.0 || tex_coord.y > 1.0 || tex_coord.x < 0.0 || tex_coord.y < 0.0)
		discard;

	vec3 tex_color = vec3(texture(uTexture, tex_coord));
	vec3 normal = vec3(texture(uNormalTex, tex_coord));
	normal = normalize(normal * 2.0 - 1.0);

	vec3 light_dir = normalize(fs_in.v_TangentLightPos - fs_in.v_TangentFragPos);

	vec3 ambient = uPointLight.ambient * tex_color;

	float distance = length(fs_in.v_TangentLightPos - fs_in.v_TangentFragPos);
	float attenuation = 1.0 / (uPointLight.constant + uPointLight.linear * distance + uPointLight.quadratic * distance * distance);

	float diff = max(0, dot(light_dir, normal));
	vec3 diffuse = attenuation * diff * uPointLight.diffuse * tex_color;

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(0, dot(reflect_dir, view_dir)), uShinness);
	vec3 specular = attenuation * spec * vec3(0.2);

	color = vec4(ambient + diffuse + specular, 1.0);
}