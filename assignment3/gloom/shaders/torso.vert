#version 430 core
layout(location = 2) in vec3 position;
layout(location = 6) in vec4 colorTorso;
out vec4 interColorTorso;
uniform layout(location = 4) mat4x4 MVP;

void main()
{
mat4x4 matrix = mat4(1);
interColorTorso = colorTorso;
gl_Position = MVP* vec4(position, 1.0f)*matrix;

}