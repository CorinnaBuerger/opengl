#version 330 core
out vec4 frag_color;

uniform vec3 u_lighting_color;

void main()
{
    frag_color = vec4(u_lighting_color, 1.0);
}
