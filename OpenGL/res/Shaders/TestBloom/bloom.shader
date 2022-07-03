# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
	vec2 texCoord;
} vs_out;


uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	vs_out.texCoord = texCoord;
	gl_Position = vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

out vec4 color;


in VS_OUT
{
	vec2 texCoord;
} fs_in;

uniform sampler2D uTexture;
uniform bool uIsHorizontal;

uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 tex_offset = 1.0 / textureSize(uTexture, 0);
    vec3 result = texture(uTexture, fs_in.texCoord).rgb * weight[0];
    if (uIsHorizontal)
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(uTexture, fs_in.texCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(uTexture, fs_in.texCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(uTexture, fs_in.texCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(uTexture, fs_in.texCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
	color = vec4(result, 1.0);
}