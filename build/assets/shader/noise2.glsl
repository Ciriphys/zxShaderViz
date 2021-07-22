@vertex
#version 460 core

layout(location = 0) in vec2 a_Pos;

out vec4 auxiliaryColor;

void main()
{
    gl_Position = vec4(a_Pos, 0.0f, 1.0f);
}


















@fragment

