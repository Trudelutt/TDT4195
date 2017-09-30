#version 430 core
in vec4 interColor;
out vec4 fragmentColor;



void main()
{			
	fragmentColor = interColor;
    //color = vec4(0.8f, 0.5f, 0.3f, 1.0f); // make the triangl(es) orange
}
