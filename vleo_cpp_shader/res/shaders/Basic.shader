#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in uint aColor;

flat out uint vColor;

uniform mat4 u_MVP;

void main()
{
    vColor = aColor;
    gl_Position = u_MVP * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
flat in uint vColor;
out uvec4 FragColor;

void main()
{
    FragColor = uvec4(vColor, 0u, 0u, 255u); // Orange color

}