# vertex shader

#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 uProj;
uniform mat4 uView;

out VS_OUT
{
	vec3 localPos;
} vs_out;

void main()
{
	vs_out.localPos = pos;
	gl_Position = uProj * uView * vec4(pos, 1.0);
}

# fragment shader

#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 localPos;
}fs_in;

uniform sampler2D uEquirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
    vec2 uv = SampleSphericalMap(normalize(fs_in.localPos)); // make sure to normalize localPos
    vec3 color = texture(uEquirectangularMap, uv).rgb;

    FragColor = vec4(color, 1.0);
}