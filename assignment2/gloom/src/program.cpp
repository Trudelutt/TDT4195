// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
//global
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

// Function for seting up VAO
GLuint setupVAO(float vertices[], unsigned int vertlength, unsigned int indices[], unsigned int indiceslength, float colours[], unsigned int colourlength);
void updateMVP();
void cameraMovement(GLFWwindow* window);

//Function to draw the scene
void drawScene(GLsizei element, unsigned int vaoID);
//motion is the position to the carmera
glm::vec3 motion = glm::vec3(0, 0, 0);
glm::mat4x4 projection = glm::perspective(glm::radians(120.0), 4.0 / 3.0, 1.0, 100.0);
glm::mat4 Model = glm::mat4(1.0f);
glm::mat4x4 Scale = glm::mat4(1.0f);
glm::mat4 Rotation = glm::mat4(1.0f);
//sat to rotate around x-axis by defaulte
glm::vec3 Rotationaxis = glm::vec3(1, 0, 0);
float angle = 0;
//the tranformation matrix
glm::mat4 MVP;



void runProgram(GLFWwindow* window)
{
	//local variables
	unsigned int vaoID;
	// Enable depth (Z) buffer (accept "closest" fragment)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		0.4, 0.5, 0.0,
		0.5, 0.3, 0.0,//7

		0.1, -0.1, 0.0,
		0.3, -0.1, 0.0,//8
		0.2, 0.1, 0.0,//9

		-0.3, -0.1, 0.0,//10
		-0.1, -0.1, 0.0,
		-0.2, 0.1, 0.0//11
	};
	//It's the indices to the 5 triangles
	unsigned int indices[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
	};
	//colors for five triangles
	float colours[] = {
		0.0, 0.0, 0.0, 1.0, //0
		0.0, 0.0, 0.0, 1.0, //1
		0.0, 0.0, 0.0, 1.0, //2

		1.0, 1.0, 1.0,  1.0, //3
		1.0, 1.0, 1.0,  1.0, //4
		1.0, 1.0, 1.0,  1.0, //5

		0.8, 0.5, 0.3, 1.0, //6
		0.8, 0.5, 0.3, 1.0, //7
		0.8, 0.5, 0.3, 1.0, //8

		0.0, 1.0, 0.0, 1.0, //9
		0.0, 1.0, 0.0, 1.0, // 10
		0.0, 1.0, 0.0, 1.0, //11

		0.8, 0.0, 1.0, 1.0, //12
		0.8, 0.0, 1.0, 1.0, //13
		0.8, 0.0, 1.0, 1.0 //14
	};
	// vertices for one triangle
	float verticesOneTriangle[] = {
		0.6, -0.8, -1.2, //0
		0, 0.4, 0, //1
		-0.8, -0.2, 1.2
	};
	//indices for one triangle
	unsigned int indicesOneTriangle[] = {
		0, 1, 2
	};
	//color for one triangle
	float colour[] = {
		1.0, 1.0, 1.0, 0.8,
		1.0, 1.0, 1.0, 0.8,
		1.0, 1.0, 1.0, 0.8
	};
	//vertices for three triangles
	float threeVertices[] = {
		-0.3, -0.4, -4,
		0.1, -0.4, -4,
		0.0, 0.1, -4,
		
		-0.1, 0.0, -3,
		0.4, 0.0, -3,
		0.2, 0.6, -3,
		
		-0.6, 0.0, -2,
		0.2, 0.0, -2,
		0.6, 0.5, -2,
	};
	//indices for three triangles
	unsigned int threeIndices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8
	};
	//colors for three triangles
	float threeColor[] = {
		0.0, 1.0, 0.0, 0.5, //0
		0.0, 1.0, 0.0, 0.5, //1
		0.0, 1.0, 0.0, 0.5, //2

		1.0, 0.0, 0.0,  0.5, //3
		1.0, 0.0, 0.0,  0.5, //4
		1.0, 0.0, 0.0,  0.5, //5	

		0.0, 0.0, 1.0, 0.5, //6
		0.0, 0.0, 1.0, 0.5, //7
		0.0, 0.0, 1.0, 0.5, //8
	};	
	// task 1
	//vaoID = setupVAO(vertices, 15 * 3, indices, 15, colours, 15 * 4);
	vaoID = setupVAO(threeVertices, 9 * 3, threeIndices, 9, threeColor, 9 * 4);
	// Rendering Loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//vaoID = setupVAO(verticesOneTriangle, 9, indicesOneTriangle, 3, colour, 3 * 4);
		// Draw your scene here
		glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(MVP));
		glBindVertexArray(vaoID);
		drawScene(9, vaoID);
		updateMVP();
		// Handle other events
		glfwPollEvents();
		handleKeyboardInput(window);
		cameraMovement(window);
		// Flip buffers
		glfwSwapBuffers(window);
	}
}


GLuint setupVAO(float* vertices, unsigned int vertlength, unsigned int* indices, unsigned int indiceslength,
	float* colours, unsigned int colourlength
) {
	GLuint vaoID = 0;
	GLuint vboID = 0;
	GLuint iboID = 1;
	GLuint cboID = 2;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	//VBO
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertlength, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	//IBO
	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indiceslength, indices, GL_STATIC_DRAW);
	//CBO
	glGenBuffers(1, &cboID);
	glBindBuffer(GL_ARRAY_BUFFER, cboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*colourlength, colours, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	return vaoID;
}

void drawScene(GLsizei element, unsigned int vaoID) {
	//Draw the triangles
	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, element, GL_UNSIGNED_INT, nullptr);
	updateMVP();	
}

void handleKeyboardInput(GLFWwindow* window)
{
	// Use escape key for terminating the GLFW window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void updateMVP() {
	glm::mat4 View = glm::translate(-motion)* Rotation* Scale;
	MVP = projection * View * Model;
}

void cameraMovement(GLFWwindow* window) {
	float speed = 0.01;
	//use a to move forward on the x-axias and s to move backwards
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		motion.x += speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		motion.x -= speed;
	}
	//use n to move forward on y-axis and m to move backwards
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		motion.y += speed;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		motion.y -= speed;
	}
	// Use i to zoom and o to zoom out
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		motion.z += speed;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		motion.z -= speed;
	}
	if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
		Scale = glm::scale(glm::vec3(Scale[0][0] + speed, Scale[0][0] + speed, Scale[0][0] + speed));
	}
	if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
		Scale = glm::scale(glm::vec3(Scale[0][0] - speed, Scale[0][0] - speed, Scale[0][0] - speed));
	}
	//rotion forword on the x-axis
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		Rotationaxis = glm::vec3(0, 1, 0);
		angle += speed;
	}
	//rotion backwards on the x-axis
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		Rotationaxis = glm::vec3(0, 1, 0);
		angle -= speed;
	}
	//rotion up on the y-axis
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		Rotationaxis = glm::vec3(1, 0, 0);
		angle += speed;
	}
	//rotion down on the y-axis
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		Rotationaxis = glm::vec3(1, 0, 0);
		angle -= speed;
	}
	//rotion forword on the z-axis
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		Rotationaxis = glm::vec3(0, 0, 1);
		angle += speed;
	}
	//rotion backwards on the z-axis
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		Rotationaxis = glm::vec3(0, 0, 1);
		angle -= speed;
	}
	Rotation = glm::rotate(angle, Rotationaxis);
}