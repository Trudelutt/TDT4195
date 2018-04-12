// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "sceneGraph.hpp"
#include "gloom/camera.hpp";
//global
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <sstream>

// Function for seting up VAO
GLuint setupVAO(float vertices[], unsigned int vertlength, unsigned int indices[], unsigned int indiceslength, float colours[], unsigned int colourlength);
void updateMVP();
void cameraMovement(GLFWwindow* window);
SceneNode* setupSceneGraph();
SceneNode* makeSceneGraph(float* vertices, unsigned int vertlength, unsigned int* indices, unsigned int indiceslength,
	float* redcolours, float* bluecolours, unsigned int colourlength);
void updateSceneGraph(SceneNode* root, float time_elapsed);
void renderSceneGraph(SceneNode* root);
SceneNode* rootBoard;
SceneNode* head = createSceneNode();
SceneNode* torso = createSceneNode();
SceneNode* leftarm = createSceneNode();
SceneNode* rightarm = createSceneNode();
SceneNode* leftleg = createSceneNode();
SceneNode* rightleg = createSceneNode();
std::stack<glm::mat4>* stack = createEmptyMatrixStack();

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
	unsigned int vaoIDHead;
	unsigned int vaoIDTorso;
	unsigned int vaoIDLeftArm;
	unsigned int vaoIDRightArm;
	unsigned int vaoIDLeftLeg;
	unsigned int vaoIDRigthLeg;

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


	//vertices head
	float vertices[] = {
		// front face
		-0.125, 0.3, -1, //0 buttom left corner
		0.125, 0.3, -1, //1 buttom right corner
		-0.125, 0.55, -1, //2 top left corner
		0.125, 0.55, -1, //3 top rigth corner

		//top face
		0.125,  0.55, -1.25, //4
		-0.125, 0.55, -1.25, // 5
		-0.125, 0.55, -1, //6
		0.125, 0.55, -1, //7

		//Bottom face
		0.125,  0.3, -1, //8
		-0.125, 0.3, -1, //9 
		-0.125, 0.3, -1.25, // 10
		0.125, 0.3, -1.25, //11

		//left face
		-0.125,  0.55, -1, //12
		-0.125, 0.55, -1.25, //13 
		-0.125, 0.3, -1.25, // 14
		-0.125, 0.3, -1, //15

		//rigth face
		0.125,  0.55, -1.25, //16
		0.125, 0.55, -1, //17 
		0.125, 0.3, -1, //18
		0.125, 0.3, -1.25, //19

		//back face
		0.125, 0.3, -1.25, //20
		-0.125, 0.3, -1.25, //21
		-0.125, 0.55, -1.25, //22
		0.125, 0.55, -1.25 //23





	};
	//It's the indices to the head
	unsigned int indices[] = {
		//front
		0, 1, 2,
		1, 3, 2,
		//top
		4, 5, 6,
		6, 7, 4,
		//bottom
		10, 11, 8,
		10, 8, 9,
		//left
		14, 15, 12,
		14, 12, 13,
		//rigth
		18, 19, 16,
		18, 16, 17,

		//back
		20, 21, 22,
		22, 23, 20
	};
	//colors for the head
	float colours[] = {
		//front
		1.0, 0.894, 0.710, 1.0, //0
		1.0, 0.894, 0.710, 1.0, //1
		1.0, 0.894, 0.710, 1.0, //2
		1.0, 0.894, 0.710, 1.0, //3
		//top
		0.0, 0.0, 0.0, 1.0, //4
		0.0, 0.0, 0.0, 1.0, //5
		0.0, 0.0, 0.0, 1.0, //6
		0.0, 0.0, 0.0, 1.0, // 7
		//buttom
		1.0, 0.894, 0.710, 1.0, //8
		1.0, 0.894, 0.710, 1.0, //9
		1.0, 0.894, 0.710, 1.0, //10
		1.0, 0.894, 0.710, 1.0, //11
		//left
		0.0, 0.0, 0.0, 1.0, //12
		0.0, 0.0, 0.0, 1.0, //13
		0.0, 0.0, 0.0, 1.0, //14
		0.0, 0.0, 0.0, 1.0, // 15
		//rigth
		0.0, 0.0, 0.0, 1.0, //16
		0.0, 0.0, 0.0, 1.0, //17
		0.0, 0.0, 0.0, 1.0, //18
		0.0, 0.0, 0.0, 1.0, // 19
		//back
		0.0, 0.0, 0.0, 1.0, //20
		0.0, 0.0, 0.0, 1.0, //21
		0.0, 0.0, 0.0, 1.0, //22
		0.0, 0.0, 0.0, 1.0 // 23
	};

	float torsoVertices[] = {
		// front face
		-0.125, -0.1, -1.0625, //0 buttom left corner
		0.125, -0.1, -1.0625, //1 buttom right corner
		-0.125, 0.3, -1.0625, //2 top left corner
		0.125, 0.3, -1.0625, //3 top rigth corner
		//top face
		0.125,  0.3, -1.1875, //4
		-0.125, 0.3, -1.1875, // 5
		-0.125, 0.3, -1.0625, //6
		0.125, 0.3, -1.0625, //7

		//Bottom face
		0.125,  -0.1, -1.0625, //8
		-0.125, -0.1, -1.0625, //9 
		-0.125, -0.1, -1.1875, // 10
		0.125, -0.1, -1.1875, //11

		//left face
		-0.125,  0.3, -1.0625, //12
		-0.125, 0.3, -1.1875, //13 
		-0.125, -0.1, -1.1875, // 14
		-0.125, -0.1, -1.0625, //15

		//rigth face
		0.125,  0.3, -1.1875, //16
		0.125, 0.3, -1.0625, //17 
		0.125, -0.1, -1.0625, //18
		0.125, -0.1, -1.1875, //19

		//back face
		0.125, -0.1, -1.1875, //20
		-0.125, -0.1, -1.1875, //21
		-0.125, 0.3, -1.1875, //22
		0.125, 0.3, -1.1875 //23                                                                                                                                                                                   
	};
	//It's the indices to the head
	unsigned int torsoIndices[] = {
		//front
		0, 1, 2,
		1, 3, 2,
		//top
		4, 5, 6,
		6, 7, 4,
		//bottom
		10, 11, 8,
		10, 8, 9,
		//left
		14, 15, 12,
		14, 12, 13,
		//rigth
		18, 19, 16,
		18, 16, 17,

		//back
		20, 21, 22,
		22, 23, 20
	};
	//colors for the head
	float torsoColours[] = {
		//front
		1.0, 0.0, 0.0, 1.0, //0
		1.0, 0.0, 0.0, 1.0, //1
		1.0, 0.0, 0.0, 1.0, //2
		1.0, 0.0, 0.0, 1.0, //3
		//top
		1.0, 0.0, 0.0, 1.0, //4
		1.0, 0.0, 0.0, 1.0, //5
		1.0, 0.0, 0.0, 1.0, //6
		1.0, 0.0, 0.0, 1.0, // 7
		//buttom
		1.0, 0.0, 0.0, 1.0, //8
		1.0, 0.0, 0.0, 1.0, //9
		1.0, 0.0, 0.0, 1.0, //10
		1.0, 0.0, 0.0, 1.0, //11
		//left
		1.0, 0.0, 1.0, 1.0, //12
		1.0, 0.0, 1.0, 1.0, //13
		1.0, 0.0, 1.0, 1.0, //14
		1.0, 0.0, 1.0, 1.0, // 15
		//rigth
		1.0, 0.0, 1.0, 1.0, //16
		1.0, 0.0, 1.0, 1.0, //17
		1.0, 0.0, 1.0, 1.0, //18
		1.0, 0.0, 1.0, 1.0, // 19
		//back
		1.0, 0.0, 1.0, 1.0, //20
		1.0, 0.0, 1.0, 1.0, //21
		1.0, 0.0, 1.0, 1.0, //22
		1.0, 0.0, 1.0, 1.0 // 23
	};
	float leftarmVertices[] = {
		// front face
		-0.255, -0.1, -1.0625, //0 buttom left corner
		-0.125, -0.1, -1.0625, //1 buttom right corner
		-0.255, 0.3, -1.0625, //2 top left corner
		-0.125, 0.3, -1.0625, //3 top rigth corner
		//top face
		-0.125,  0.3, -1.1875, //4
		-0.255, 0.3, -1.1875, // 5
		-0.255, 0.3, -1.0625, //6
		-0.125, 0.3, -1.0625, //7
		//Bottom face
		-0.125,  -0.1, -1.0625, //8
		-0.255, -0.1, -1.0625, //9 
		-0.255, -0.1, -1.1875, // 10
		-0.125, -0.1, -1.1875, //11
		//left face
		-0.255,  0.3, -1.0625, //12
		-0.255, 0.3, -1.1875, //13 
		-0.255, -0.1, -1.1875, // 14
		-0.255, -0.1, -1.0625, //15
		//rigth face
		-0.125,  0.3, -1.1875, //16
		-0.125, 0.3, -1.0625, //17 
		-0.125, -0.1, -1.0625, //18
		-0.125, -0.1, -1.1875, //19
		//back face
		-0.125, -0.1, -1.1875, //20
		-0.255, -0.1, -1.1875, //21
		-0.255, 0.3, -1.1875, //22
		-0.125, 0.3, -1.1875 //23  
	};
	//It's the indices to the left arm
	unsigned int leftarmIndices[] = {
		//front
		0, 1, 2,
		1, 3, 2,
		//top
		4, 5, 6,
		6, 7, 4,
		//bottom
		10, 11, 8,
		10, 8, 9,
		//left
		14, 15, 12,
		14, 12, 13,
		//rigth
		18, 19, 16,
		18, 16, 17,
		//back
		20, 21, 22,
		22, 23, 20
	};
	//colors for the head
	float leftarmColours[] = {
		//front
		1.0, 0.0, 0.0, 1.0, //0
		1.0, 0.0, 0.0, 1.0, //1
		1.0, 0.0, 0.0, 1.0, //2
		1.0, 0.0, 0.0, 1.0, //3
		//top
		1.0, 0.0, 0.0, 1.0, //4
		1.0, 0.0, 0.0, 1.0, //5
		1.0, 0.0, 0.0, 1.0, //6
		1.0, 0.0, 0.0, 1.0, // 7
		//buttom
		1.0, 0.0, 0.0, 1.0, //8
		1.0, 0.0, 0.0, 1.0, //9
		1.0, 0.0, 0.0, 1.0, //10
		1.0, 0.0, 0.0, 1.0, //11
		//left
		1.0, 0.0, 1.0, 1.0, //12
		1.0, 0.0, 1.0, 1.0, //13
		1.0, 0.0, 1.0, 1.0, //14
		1.0, 0.0, 1.0, 1.0, // 15
		//rigth
		1.0, 0.0, 1.0, 1.0, //16
		1.0, 0.0, 1.0, 1.0, //17
		1.0, 0.0, 1.0, 1.0, //18
		1.0, 0.0, 1.0, 1.0, // 19
		//back
		1.0, 0.0, 1.0, 1.0, //20
		1.0, 0.0, 1.0, 1.0, //21
		1.0, 0.0, 1.0, 1.0, //22
		1.0, 0.0, 1.0, 1.0 // 23
	};
	float rigtharmVertices[] = {
		// front face
		0.125, -0.1, -1.0625, //0 buttom left corner
		0.255, -0.1, -1.0625, //1 buttom right corner
		0.125, 0.3, -1.0625, //2 top left corner
		0.255, 0.3, -1.0625, //3 top rigth corner
		//top face
		0.255,  0.3, -1.1875, //4
		0.125, 0.3, -1.1875, // 5
		0.125, 0.3, -1.0625, //6
		0.255, 0.3, -1.0625, //7


		//Bottom face
		0.255,  -0.1, -1.0625, //8
		0.125, -0.1, -1.0625, //9 
		0.125, -0.1, -1.1875, // 10
		0.255, -0.1, -1.1875, //11
		//left face
		0.125,  0.3, -1.0625, //12
		0.125, 0.3, -1.1875, //13 
		0.125, -0.1, -1.1875, // 14
		0.125, -0.1, -1.0625, //15
		//rigth face
		0.255,  0.3, -1.1875, //16
		0.255, 0.3, -1.0625, //17 
		0.255, -0.1, -1.0625, //18
		0.255, -0.1, -1.1875, //19
		//back face
		0.255, -0.1, -1.1875, //20
		0.125, -0.1, -1.1875, //21
		0.125, 0.3, -1.1875, //22
		0.255, 0.3, -1.1875 //23  
	};
	//It's the indices to the left arm
	unsigned int rigtharmIndices[] = {
		//front
		0, 1, 2,
		1, 3, 2,
		//top
		4, 5, 6,
		6, 7, 4,
		//bottom
		10, 11, 8,
		10, 8, 9,
		//left
		14, 15, 12,
		14, 12, 13,
		//rigth
		18, 19, 16,
		18, 16, 17,
		//back
		20, 21, 22,
		22, 23, 20
	};
	//colors for the head
	float rigtharmColours[] = {
		//front
		1.0, 0.0, 0.0, 1.0, //0
		1.0, 0.0, 0.0, 1.0, //1
		1.0, 0.0, 0.0, 1.0, //2
		1.0, 0.0, 0.0, 1.0, //3
		//top
		1.0, 0.0, 0.0, 1.0, //4
		1.0, 0.0, 0.0, 1.0, //5
		1.0, 0.0, 0.0, 1.0, //6
		1.0, 0.0, 0.0, 1.0, // 7
		//buttom
		1.0, 0.0, 0.0, 1.0, //8
		1.0, 0.0, 0.0, 1.0, //9
		1.0, 0.0, 0.0, 1.0, //10
		1.0, 0.0, 0.0, 1.0, //11
		//left
		1.0, 0.0, 1.0, 1.0, //12
		1.0, 0.0, 1.0, 1.0, //13
		1.0, 0.0, 1.0, 1.0, //14
		1.0, 0.0, 1.0, 1.0, // 15
		//rigth
		1.0, 0.0, 1.0, 1.0, //16
		1.0, 0.0, 1.0, 1.0, //17
		1.0, 0.0, 1.0, 1.0, //18
		1.0, 0.0, 1.0, 1.0, // 19
		//back
		1.0, 0.0, 1.0, 1.0, //20
		1.0, 0.0, 1.0, 1.0, //21
		1.0, 0.0, 1.0, 1.0, //22
		1.0, 0.0, 1.0, 1.0 // 23
	};

	float leftlegVertices[] = {
		// front face
		-0.125, -0.4, -1.0625, //0 buttom left corner
		0.0, -0.4, -1.0625, //1 buttom right corner
		-0.125, -0.1, -1.0625, //2 top left corner
		0.0, -0.1, -1.0625, //3 top rigth corner
		//top face
		0.0,  -0.1, -1.1875, //4
		-0.125, -0.1, -1.1875, // 5
		-0.125, -0.1, -1.0625, //6
		0.0, -0.1, -1.0625, //7

		//Bottom face
		0.0,  -0.4, -1.0625, //8
		-0.125, -0.4, -1.0625, //9 
		-0.125, -0.4, -1.1875, // 10
		0.0, -0.4, -1.1875, //11

		//left face
		-0.125,  -0.1, -1.0625, //12
		-0.125, -0.1, -1.1875, //13 
		-0.125, -0.4, -1.1875, // 14
		-0.125, -0.4, -1.0625, //15

		//rigth face
		0.0,  -0.1, -1.1875, //16
		0.0, -0.1, -1.0625, //17 
		0.0, -0.4, -1.0625, //18
		0.0, -0.4, -1.1875, //19

		//back face
		0.0, -0.4, -1.1875, //20
		-0.125, -0.4, -1.1875, //21
		-0.125, -0.1, -1.1875, //22
		0.0, -0.1, -1.1875 //23       
	};
	//It's the indices to the left arm
	unsigned int leftlegIndices[] = {
		//front
		0, 1, 2,
		1, 3, 2,
		//top
		4, 5, 6,
		6, 7, 4,
		//bottom
		10, 11, 8,
		10, 8, 9,
		//left
		14, 15, 12,
		14, 12, 13,
		//rigth
		18, 19, 16,
		18, 16, 17,
		//back
		20, 21, 22,
		22, 23, 20
	};
	//colors for the head
	float leftlegColours[] = {
		//front
		0.0, 0.0, 1.0, 1.0, //0
		0.0, 0.0, 1.0, 1.0, //1
		0.0, 0.0, 1.0, 1.0, //2
		0.0, 0.0, 1.0, 1.0, //3
		//top
		0.0, 0.0, 1.0, 1.0, //4
		0.0, 0.0, 1.0, 1.0, //5
		0.0, 0.0, 1.0, 1.0, //6
		0.0, 0.0, 1.0, 1.0, // 7
		//buttom
		0.0, 0.0, 1.0, 1.0, //8
		0.0, 0.0, 1.0, 1.0, //9
		0.0, 0.0, 1.0, 1.0, //10
		0.0, 0.0, 1.0, 1.0, //11
		//left
		0.0, 1.0, 1.0, 1.0, //12
		0.0, 1.0, 1.0, 1.0, //13
		0.0, 1.0, 1.0, 1.0, //14
		0.0, 1.0, 1.0, 1.0, // 15
		//rigth
		0.0, 1.0, 1.0, 1.0, //16
		0.0, 1.0, 1.0, 1.0, //17
		0.0, 1.0, 1.0, 1.0, //18
		0.0, 1.0, 1.0, 1.0, // 19
		//back
		0.0, 1.0, 1.0, 1.0, //20
		0.0, 1.0, 1.0, 1.0, //21
		0.0, 1.0, 1.0, 1.0, //22
		0.0, 1.0, 1.0, 1.0 // 23
	};
	float rigthlegVertices[] = {
		// front face
		0.0, -0.4, -1.0625, //0 buttom left corner
		0.125, -0.4, -1.0625, //1 buttom right corner
		0.0, -0.1, -1.0625, //2 top left corner
		0.125, -0.1, -1.0625, //3 top rigth corner
		//top face
		0.125,  -0.1, -1.1875, //4
		0.0, -0.1, -1.1875, // 5
		0.0, -0.1, -1.0625, //6
		0.125, -0.1, -1.0625, //7

		//Bottom face
		0.125,  -0.4, -1.0625, //8
		0.0, -0.4, -1.0625, //9 
		0.0, -0.4, -1.1875, // 10
		0.125, -0.4, -1.1875, //11

		//left face
		0.0,  -0.1, -1.0625, //12
		0.0, -0.1, -1.1875, //13 
		0.0, -0.4, -1.1875, // 14
		0.0, -0.4, -1.0625, //15

		//rigth face
		0.125,  -0.1, -1.1875, //16
		0.125, -0.1, -1.0625, //17 
		0.125, -0.4, -1.0625, //18
		0.125, -0.4, -1.1875, //19

		//back face
		0.125, -0.4, -1.1875, //20
		0.0, -0.4, -1.1875, //21
		0.0, -0.1, -1.1875, //22
		0.125, -0.1, -1.1875 //23       
	};
	//It's the indices to the left arm
	unsigned int rigthlegIndices[] = {
		//front
		0, 1, 2,
		1, 3, 2,
		//top
		4, 5, 6,
		6, 7, 4,
		//bottom
		10, 11, 8,
		10, 8, 9,
		//left
		14, 15, 12,
		14, 12, 13,
		//rigth
		18, 19, 16,
		18, 16, 17,
		//back
		20, 21, 22,
		22, 23, 20
	};
	//colors for the head
	float rigthlegColours[] = {
		//front
		0.0, 0.0, 1.0, 1.0, //0
		0.0, 0.0, 1.0, 1.0, //1
		0.0, 0.0, 1.0, 1.0, //2
		0.0, 0.0, 1.0, 1.0, //3
		//top
		0.0, 0.0, 1.0, 1.0, //4
		0.0, 0.0, 1.0, 1.0, //5
		0.0, 0.0, 1.0, 1.0, //6
		0.0, 0.0, 1.0, 1.0, // 7
		//buttom
		0.0, 0.0, 1.0, 1.0, //8
		0.0, 0.0, 1.0, 1.0, //9
		0.0, 0.0, 1.0, 1.0, //10
		0.0, 0.0, 1.0, 1.0, //11
		//left
		0.0, 1.0, 1.0, 1.0, //12
		0.0, 1.0, 1.0, 1.0, //13
		0.0, 1.0, 1.0, 1.0, //14
		0.0, 1.0, 1.0, 1.0, // 15
		//rigth
		0.0, 1.0, 1.0, 1.0, //16
		0.0, 1.0, 1.0, 1.0, //17
		0.0, 1.0, 1.0, 1.0, //18
		0.0, 1.0, 1.0, 1.0, // 19
		//back
		0.0, 1.0, 1.0, 1.0, //20
		0.0, 1.0, 1.0, 1.0, //21
		0.0, 1.0, 1.0, 1.0, //22
		0.0, 1.0, 1.0, 1.0 // 23
	};
	float verticesboard[] = {
		-0.6, -0.4, -1.43,
		-0.43, -0.4, -1.43,
		-0.6, -0.4, -1.6,
		-0.43, -0.4, -1.6
	};

	unsigned int indicesBoard[] = {
		0,1,2,
		1, 3, 2

	};

	float colorredboard[] = {
	 1.0, 0.0, 0.0, 0.5,
	 1.0, 0.0, 0.0, 0.5,
	 1.0, 0.0, 0.0, 0.5,
	 1.0, 0.0, 0.0, 0.5
	};

	float colorblueboard[] = {
		0.0, 0.0, 1.0, 0.5,
		0.0, 0.0, 1.0, 0.5,
		0.0, 0.0, 1.0, 0.5,
		0.0, 0.0, 1.0, 0.5
	};


	// task 1
	vaoIDHead = setupVAO(vertices, 24 * 3, indices, 3 * 13, colours, 24 * 4);
	head->vertexArrayObjectID = vaoIDHead;
	vaoIDTorso = setupVAO(torsoVertices, 24 * 3, torsoIndices, 3 * 13, torsoColours, 24 * 4);
	torso->vertexArrayObjectID = vaoIDTorso;
	vaoIDLeftArm = setupVAO(leftarmVertices, 24 * 3, leftarmIndices, 3 * 13, leftarmColours, 24 * 4);
	leftarm->vertexArrayObjectID = vaoIDLeftArm;
	vaoIDRightArm = setupVAO(rigtharmVertices, 24 * 3, rigtharmIndices, 3 * 13, rigtharmColours, 24 * 4);
	rightarm->vertexArrayObjectID = vaoIDRightArm;
	vaoIDLeftLeg = setupVAO(leftlegVertices, 24 * 3, leftlegIndices, 3 * 13, leftlegColours, 24 * 4);
	leftleg->vertexArrayObjectID = vaoIDLeftLeg;
	vaoIDRigthLeg = setupVAO(rigthlegVertices, 24 * 3, rigthlegIndices, 3 * 13, rigthlegColours, 24 * 4);
	rightleg->vertexArrayObjectID = vaoIDRigthLeg;
	rootBoard = makeSceneGraph(verticesboard, 4 * 3, indicesBoard, 6, colorredboard, colorblueboard, 4 * 4);
	SceneNode* root = setupSceneGraph();
	std::vector<int2> coor = readCoordinatesFile("../Gloom/src/coordinates_2.txt");
	int count = 0;
	double timeX = 0;
	double timeZ = 0;
	double time;
	root->rotationDirection = 1.0;
	printf("%d\n", root->rotationDirection);
	while (!glfwWindowShouldClose(window))
	{
		time = getTimeDeltaSeconds();
		// Clear colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Draw your scene here
		if (count == coor.size() - 1) {
			root->rotationDirection = -1;
		}
		if (root->x < -coor[count].x*0.17 * 2) {
			if (root->x + time < -coor[count].x*0.17 * 2) {
				root->x = root->x + time;
			}
			else {
				timeX = 1;
			}
		}
		else {
			if (root->x - time > -coor[count].x*0.17 * 2) {
				root->x = root->x - time;
			}
			else {
				timeX = 1;
			}
		}
		if (root->z > coor[count].y*0.17 * 2) {
			if (root->z - time > coor[count].y*0.17 * 2) {
				root->z = root->z - time;
			}
			else {
				timeZ = 1;
			}
		}
		else {
			if (root->z + time < coor[count].y*0.17 * 2) {
				root->z = root->z + time;
			}
			else {
				timeZ = 1;
			}
		}


		if (timeX == 1 && timeZ == 1) {
			count+= root->rotationDirection;
			if (count == 0) {
				root->rotationDirection = 1;
			}
			timeX = 0;
			timeZ = 0;
		}
	updateSceneGraph(root, time);
	renderSceneGraph(root);
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	//IBO
	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indiceslength, indices, GL_STATIC_DRAW);
	//CBO
	glGenBuffers(1, &cboID);
	glBindBuffer(GL_ARRAY_BUFFER, cboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*colourlength, colours, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(3);
	return vaoID;
}

void drawScene(GLsizei element, unsigned int vaoID) {
	//Draw the triangles
	updateMVP();
	glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(MVP));
	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, element, GL_UNSIGNED_INT, nullptr);
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
	Gloom::Camera camera;
	glm::mat4 View = Scale*Rotation*glm::translate(-motion);
	MVP = projection * View*Model;
}

SceneNode* makeSceneGraph(float* vertices, unsigned int vertlength, unsigned int* indices, unsigned int indiceslength,
	float* redcolours, float* bluecolours, unsigned int colourlength) {
	SceneNode* root = createSceneNode();
	SceneNode* row;
	root->vertexArrayObjectID = setupVAO(vertices, vertlength, indices, indiceslength, bluecolours, colourlength);
	for (int i = 0; i < 5; i++) {
		if (!i == 0) {
			row = createSceneNode();
			if (i % 2 == 0) {
				row->vertexArrayObjectID = setupVAO(vertices, vertlength, indices, indiceslength, bluecolours, colourlength);
			}
			else {
				row->vertexArrayObjectID = setupVAO(vertices, vertlength, indices, indiceslength, redcolours, colourlength);

			}
			addChild(root, row);
			row->referencePoint = glm::vec3(0, 0, i * 0.17*2);
		}
		else {
			row = root;
		}
		for (int j = 1; j < 7; j++) {
			SceneNode* col = createSceneNode();
			if (i % 2 == 0 && j % 2 != 0) {
				col->vertexArrayObjectID = setupVAO(vertices, vertlength, indices, indiceslength, redcolours, colourlength);
			}
			else {
				if (i % 2 == 0 && j % 2 == 0) {
					col->vertexArrayObjectID = setupVAO(vertices, vertlength, indices, indiceslength, bluecolours, colourlength);
				}
				else if (j % 2 == 0) {
					col->vertexArrayObjectID = setupVAO(vertices, vertlength, indices, indiceslength, redcolours, colourlength);
				}
				else {
					col->vertexArrayObjectID = setupVAO(vertices, vertlength, indices, indiceslength, bluecolours, colourlength);
				}
			}
			col->referencePoint = glm::vec3(j*0.17*2, 0, 0);
			addChild(row, col);
		}
	}
	return root;
}
SceneNode* setupSceneGraph() {
	SceneNode* root = createSceneNode();
	addChild(root, rootBoard);
	addChild(root, head);
	head->referencePoint=glm::vec3(-1.0, -0.4, -0.6);
	addChild(head, torso);
	leftarm->motionType = LEFT_ARM_MOTION;
	addChild(torso, leftarm);
	leftarm->referencePoint = glm::vec3(0.0, 0.0, 0);
	
	rightarm->motionType = RIGHT_ARM_MOTION;
	addChild(torso, rightarm);
	leftleg->motionType = LEFT_LEG_MOTION;
	addChild(torso, leftleg);
	rightarm->motionType = RIGHT_LEG_MOTION;
	addChild(torso, rightleg);
	return root;
}

void updateSceneGraph(SceneNode* root,  float time_elapsed) {
	SceneNode* child;
	float angle = toRadians(45);
	for (int i = 0; i < root->children.size(); i++) {
		child = root->children[i];
		if (child->vertexArrayObjectID == 7) {
			child->x = 0 - child->referencePoint[0];
			child->y = 0- child->referencePoint[1];
			child->z = 0 - child->referencePoint[2];
		}
		else {
			child->x = root->x - child->referencePoint[0];
			child->y = root->y - child->referencePoint[1];
			child->z = root->z - child->referencePoint[2];
		}
		child->rotationDirection = root->rotationDirection;
		updateSceneGraph(child, time_elapsed);
	}
	glm::vec3 root_pos = glm::vec3(root->x, root->y, root->z);
	glm::mat4 trans = glm::translate(glm::vec3(-root_pos));
	if (root->vertexArrayObjectID>-1 && root->vertexArrayObjectID<7) {
		glm::mat4 selfRotation = glm::mat4(1.0f);
		if ((root-> motionType == LEFT_LEG_MOTION)) {
			trans = trans *peekMatrix(stack);
			if (angle < toRadians(135)) {
				angle += toRadians(time_elapsed);
			}
			
			selfRotation = glm::translate(glm::vec3(-0.0625, -0.25, -1.125))* glm::rotate(angle, glm::vec3(1, 0, 0))*glm::translate(glm::vec3(0.0625, 0.25, 1.125));
		}
		else {
			if (root->rotationDirection == 1) {
				selfRotation = glm::translate(glm::vec3(0, 0, -1))* glm::rotate(toRadians(180), glm::vec3(0, 1, 0))*glm::translate(glm::vec3(0, 0, 1));
			}
		}
		root->currentTransformationMatrix = trans*selfRotation;
		pushMatrix(stack,root->currentTransformationMatrix);
	}
	else {
		root->currentTransformationMatrix = glm::rotate(toRadians(0), glm::vec3(1,0,0))*trans;
	}
	
}

void renderSceneGraph(SceneNode* root) {
	if (root->vertexArrayObjectID > 6) {
		root->currentTransformationMatrix = root->currentTransformationMatrix*glm::scale(glm::vec3(2, 2, 2));
	}
	Model = root->currentTransformationMatrix;
	updateMVP();
	if (root->vertexArrayObjectID == -1) {
		
	}
	else if (root->vertexArrayObjectID < 7) {
		drawScene(39, root->vertexArrayObjectID);
	}
	else {
		drawScene(6,root->vertexArrayObjectID);
	}
	for (int i = 0; i < root->children.size(); i++) {
		renderSceneGraph(root->children[i]);
	}
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