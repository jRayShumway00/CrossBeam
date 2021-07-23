#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "headers/BasicShader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

// Settings:
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Bools:
bool isWireFrameOn = false;

#pragma region MAIN
int main(void)
{

#pragma region GLFW INIT
	/* GLFW: CONFIGURE & INITIALIZE */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#pragma endregion

#pragma region GLFW WINDOW CREATION
	/* GLFW: WINDOW CREATION */
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CrossBeam", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window!" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// OPENGL Viewport:
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
#pragma endregion	

#pragma region GLEW INIT
	/* GLEW: LOAD ALL OPENGL FUNCTION POINTERS */
	int glewErr = glewInit();
	if (GLEW_OK != glewErr)
	{
		/* Problem: glewInit failed, something is seriously wrong! */
		fprintf(stderr, "ERROR: %s\n", glewGetErrorString(glewErr));
	}
#pragma endregion

	// Print Current OGL Version:
	cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

#pragma region TRIANGLE CREATION
	// Vertices for Triangle!
	float vertices[] = {
		// positions #####		// colors ######
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	// top right
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	// bottom right
		 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f	// top left
	};

	// Creating Vertex Buffer Objects:
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Binding the VBO to the Array buffer:
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy Vertex Data into Memory:
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* INTERPRET VERTEX DATA */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* GENERATE VAO */
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
#pragma endregion

	// Generate Shaders:
	Shader ourShader("res/shaders/VertexShader.shader", "res/shaders/FragmentShader.shader");

#pragma region TRIANGLE INIT
	// Initialization code:
	// Bind VAO:
	glBindVertexArray(VAO);
	// copy vertices array in a buffer for OpenGL to use:
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// set vertex attribute pointers:
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// set color attribute pointers:
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
#pragma endregion

	/* RENDER LOOP */
	while (!glfwWindowShouldClose(window))
	{
		// Calls input processor:
		ProcessInput(window);

		// render
		// clear the color buffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate shader
		ourShader.use();

		// render the rectangle
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swaps the color buffer (contains color values for each pixel in GLFW Window
		glfwSwapBuffers(window);
		// Checks for triggered events (keyboard, mouse movements, etc.)
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}
#pragma endregion

// Process GLFW Input:
/* TODO:	WIREFRAME MODE */
void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	/* WIREFRAME TOGGLE (WORK IN PROGRESS) */
	if (isWireFrameOn) 
	{
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		{
			isWireFrameOn = false;
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	else if (!isWireFrameOn)
	{
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		{
			isWireFrameOn = true;
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
}

/* GLFW: Whenever the window size changes, by the OS or the user, this function will execute: */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Adjust the size:
	glViewport(0, 0, width, height);
}