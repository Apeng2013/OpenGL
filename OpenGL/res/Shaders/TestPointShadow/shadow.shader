# vertex shader

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 uModel;

void main()
{
	gl_Position = uModel * vec4(aPos, 1.0);
}


# geometry shader

#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 uProj;
uniform mat4 uLightViews[6];

out vec4 FragPos;

void main()
{
	for (int face = 0; face < 6; ++face)
	{
		gl_Layer = face;
		for (int i = 0; i < 3; ++i)
		{
			FragPos = gl_in[i].gl_Position;
			gl_Position = uProj * uLightViews[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}



# fragment shader

#version 330 core

in vec4 FragPos;

uniform vec3 uLightPos;
uniform float uFarPlane;

out vec4 color;

void main()
{
	float light_dist = length(FragPos.xyz - uLightPos);
	gl_FragDepth = light_dist / uFarPlane;
}