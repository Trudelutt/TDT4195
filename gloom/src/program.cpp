// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"

// Function for seting up VAO
GLuint setupVAO(float vertices[], unsigned int vertlength, unsigned int indices[], unsigned int indiceslength);

//Function to draw the scene
void drawScene(GLsizei element, unsigned int indices[]);

void runProgram(GLFWwindow* window)
{
	// Enable depth (Z) buffer (accept "closest" fragment)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Configure miscellaneous OpenGL settings
	glEnable(GL_CULL_FACE);

	// Set default colour after clearing the colour buffer
	glClearColor(0.3f, 0.5f, 0.8f, 1.0f);
	// Set up your scene here (create Vertex Array Objects, etc.)
	Gloom::Shader shader;
	shader.makeBasicShader("../gloom/shaders/simple.vert", "../gloom/shaders/simple.frag");
	shader.activate();
	//vertices to 5 triangles
	float vertices[] = {
		-0.1, -0.1, 0.0, //0
		0.1, -0.1, 0.0, //1
		0.0, 0.1, 0.0, //2
		0.4, 0.5, 0.0, //3
		0.2, 0.5, 0.0, //4
		0.3, 0.3, 0.0,//5
		0.6, 0.5, 0.0,//6
		0.5, 0.3, 0.0,//7
		0.3, -0.1, 0.0,//8
		0.2, 0.1, 0.0,//9
		-0.3, -0.1, 0.0,//10
		-0.2, 0.1, 0.0//11


	};
	//It's the indices to the 5 triangles
	unsigned int indices[] = {
		0, 1, 2, 3, 4, 5, 6,3, 7, 1, 8, 9, 10, 0, 11
	};
	//Task 2 
	float verticesOneTriangle[] = {
		0.6, -0.8, -1.2, //0
		0, 0.4, 0, //1
		-0.8, -0.2, 1.2
	};

	unsigned int indicesOneTriangle[] = {
		0, 1, 2
	};
	//Task 1
	//setupVAO(vertices, 12*3, indices, 15);
	// task 2
	setupVAO(verticesOneTriangle, 9, indicesOneTriangle, 3);
	
	// Rendering Loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw your scene here
		//drawScene(15, indices);
		drawScene(3, indicesOneTriangle);
		// Handle other events
		glfwPollEvents();
		handleKeyboardInput(window);

		// Flip buffers
		glfwSwapBuffers(window);
	}
}


GLuint setupVAO(float* vertices, unsigned int vertlength, unsigned int* indices, unsigned int indiceslength) {
	GLuint vaoID = 0;
	GLuint vboID = 0;
	GLuint iboID = 1;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	//VBO
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertlength, vertices, GL_STATIC_DRAW);
	//IBO
	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indiceslength, indices, GL_STATIC_DRAW);
	return vaoID;
}

void drawScene(GLsizei element, unsigned int indices[]) {
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawElements(GL_TRIANGLES, element, GL_UNSIGNED_INT, nullptr);
	glDisableVertexAttribArray(0);
}




void handleKeyboardInput(GLFWwindow* window)
{
	// Use escape key for terminating the GLFW window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}
