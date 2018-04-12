#version 430 core

layout(location = 1) in vec3 position;
layout(location = 3) in vec4 color;
out vec4 interColor;
uniform layout(location = 3) mat4x4 MVP;


void main()
{
	mat4x4 matrix = mat4(1);
	interColor = color;
	gl_Position = MVP* vec4(position, 1.0f)*matrix;
	
}
