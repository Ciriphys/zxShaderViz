@vertex
#version 460 core

layout(location = 0) in vec2 a_Pos;

out vec4 auxiliaryColor;

void main()
{
    gl_Position = vec4(a_Pos, 0.0f, 1.0f);
    //auxiliaryColor = vec4(1.0f, 0.8f, 1.0f, 1.0f);
}













































































@fragment
#version 460 core

uniform vec2 u_Resolution;
uniform vec2 u_MousePos;

in vec4 auxiliaryColor;
out vec4 oColor;

void main() 
{
	vec2 st = gl_FragCoord.xy/u_Resolution;
	oColor = vec4(st, 0.3f, 1.0f);
}





























































