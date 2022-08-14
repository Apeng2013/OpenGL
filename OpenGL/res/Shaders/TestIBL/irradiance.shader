# vertex shader

#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
	vec3 worldPos;
} vs_out;

uniform mat4 uView;
uniform mat4 uProj;

void main()
{
	vs_out.worldPos = pos;
	gl_Position = uProj * uView * vec4(pos, 1.0);
}


# fragment shader

# version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 worldPos;
} fs_in;

uniform samplerCube uEnvironmentMap;

void main()
{
	vec3 irradiance = vec3(0.0);

	float PI = 3.1415926;
	
	vec3 normal = normalize(fs_in.worldPos);
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(up, normal);
	up = cross(normal, right);

	float sampleDelta = 0.025;
	float nrSamples = 0.0;
	for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
		for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
			// spherical to cartesian (in tangent space)
			vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			// tangent space to world
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

			irradiance += texture(uEnvironmentMap, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}
	irradiance = PI * irradiance * (1.0 / float(nrSamples));

	FragColor = vec4(irradiance, 1.0);
}
