#version 430 core
in vec4 interColor;
//in vec4 interColorTorso;
//out vec4 fragmentColorTorso;
out vec4 fragmentColor;




void main()
{		
	//fragmentColorTorso = interColorTorso;
	fragmentColor = interColor;
	
 
}
