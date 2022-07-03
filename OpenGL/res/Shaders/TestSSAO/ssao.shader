# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
	vec2 texCoord;
} vs_out;

void main()
{
	vs_out.texCoord = texCoord;
	gl_Position = vec4(aPos, 1.0);
}


# fragment shader

#version 330 core

in VS_OUT
{
	vec2 texCoord;
} fs_in;

out float FragColor;

uniform sampler2D uPositionTex;
uniform sampler2D uNormalTex;
uniform sampler2D uNoiseTex;
uniform vec3 uSamples[64];
uniform mat4 uProj;

const vec2 noiseScale = vec2(960 / 4.0, 720 / 4.0);

void main()
{
	vec3 fragPos = texture(uPositionTex, fs_in.texCoord).rgb;
	vec3 normal = texture(uNormalTex, fs_in.texCoord).rgb;
	vec3 randomVec = texture(uNoiseTex, fs_in.texCoord).rgb;

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

    float radius = 1.0;
    float occlusion = 0.0;
    for (int i = 0; i < 64; ++i)
    {
        vec3 sample = TBN * uSamples[i];
        sample = fragPos + sample * radius;

        vec4 offset = vec4(sample, 1.0);
        offset = uProj * offset;
        offset.xyz /= offset.w; 
        offset.xyz = offset.xyz * 0.5 + 0.5; 

        float sampleDepth = -texture(uPositionTex, offset.xy).w;

        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= sample.z ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / 64);

    FragColor = occlusion;
}