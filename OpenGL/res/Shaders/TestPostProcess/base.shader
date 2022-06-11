# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 noraml;
layout(location = 2) in vec2 texCoord;

out vec2 v_texCoord;
uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	v_texCoord = texCoord;
	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

out vec4 color;

in vec2 v_texCoord;
uniform sampler2D uTexture;

vec4 GetInversion()
{
	//反相
	return vec4(1.0 - vec3(texture(uTexture, v_texCoord)), 1.0);
}

vec4 GetGrayscale()
{
	//灰度
	vec4 tex_color = texture(uTexture, v_texCoord);
	//float average = (tex_color.r + tex_color.g + tex_color.b) / 3.0;
	float average = 0.2126 * tex_color.r + 0.7152 * tex_color.g + 0.0722 * tex_color.b;
	return vec4(average, average, average, 1.0);
}

vec4 GetSharpenKernel()
{
	//核效果
	//锐化
	float offset = 1.0 / 300.0;
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0),
		vec2(0.0, 0.0),
		vec2(offset, 0.0),
		vec2(-offset, -offset),
		vec2(0.0, -offset),
		vec2(offset, -offset)
		);

	float kernel[9] = float[](
		-1.0, -1.0, -1.0,
		-1.0, 9.0, -1.0,
		-1.0, -1.0, -1.0
		);

	vec3 sampleTex = vec3(0.0);
	for (int i = 0; i < 9; i++)
	{
		sampleTex += vec3(texture(uTexture, v_texCoord.st + offsets[i])) * kernel[i];
	}
	return vec4(sampleTex, 1.0);
}

vec4 GetBlurKernel()
{
	//模糊
	float offset = 1.0 / 300.0;
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0),
		vec2(0.0, 0.0),
		vec2(offset, 0.0),
		vec2(-offset, -offset),
		vec2(0.0, -offset),
		vec2(offset, -offset)
		);

	float kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
		);

	vec3 sampleTex = vec3(0.0);
	for (int i = 0; i < 9; i++)
	{
		sampleTex += vec3(texture(uTexture, v_texCoord.st + offsets[i])) * kernel[i];
	}
	return vec4(sampleTex, 1.0);

}

vec4 GetEdgeDetectionKernel()
{
	//边缘检测
	//边缘高亮
	float offset = 1.0 / 300.0;
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0),
		vec2(0.0, 0.0),
		vec2(offset, 0.0),
		vec2(-offset, -offset),
		vec2(0.0, -offset),
		vec2(offset, -offset)
		);
	
	float kernel[9] = float[](
		1.0, 1.0, 1.0,
		1.0, -8.0, 1.0,
		1.0, 1.0, 1.0
		);

	vec3 sampleTex = vec3(0.0);
	for (int i = 0; i < 9; i++)
	{
		sampleTex += vec3(texture(uTexture, v_texCoord.st + offsets[i])) * kernel[i];
	}
	return vec4(sampleTex, 1.0);

}

void main()
{
	color = GetEdgeDetectionKernel();
}