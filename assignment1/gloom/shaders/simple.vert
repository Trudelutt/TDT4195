#version 430 core

in vec3 position;


void main()
{
    gl_Position = vec4(position, 1.0f);
	//gl_Position = vec4(-position.x, -position.y, position.z, 1.0f); // mirror the scene vertically and horizontally

}
