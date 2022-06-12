# vertex shader

#version 330 core
layout(location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}


# geometry shader

#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 color;

void main() {
    color = vec3(0.3, 0.3, 0.8);
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.1, 0.0, 0.0);
    color = vec3(1.0, 1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}


# fragment shader

#version 330 core

in vec3 color;

out vec4 FragColor;

void main()
{
    FragColor = vec4(color, 1.0);
}
