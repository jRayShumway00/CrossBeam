#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

// Temporary Vertex Shader Data:
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Temporary Fragment Shader Data:
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

// Settings:
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/* BEGIN MAIN FUNCTION */

int main(void)
{
	/* GLFW: CONFIGURE & INITIALIZE */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


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

	/* GLEW: LOAD ALL OPENGL FUNCTION POINTERS */
	int glewErr = glewInit();
	if (GLEW_OK != glewErr)
	{
		/* Problem: glewInit failed, something is seriously wrong! */
		fprintf(stderr, "ERROR: %s\n", glewGetErrorString(glewErr));
	}

	// Print Current OGL Version:
	cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

	// Vertices for Rectangle!
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,	// top right
		 0.5f, -0.5f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f	// top left
	};

	// Creating EBO for Rectangle:
	unsigned int indices[] = { // Start from 0!
		0, 1, 3,	// first triangle
		1, 2, 3		// second triangle
	};

	// EBO:
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Creating Vertex Buffer Objects:
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Binding the VBO to the Array buffer:
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	// Copy Vertex Data into Memory:
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	/* CREATE SHADER & COMPILE */
	// Store vertexShader as int (ID):
	unsigned int vertexShader;
	// Store fragmentShader as int (ID):
	unsigned int fragmentShader;
	// Set the value.
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Set the value.
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Attach Shader Source to Shader object and compile:
	// VertShade
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// FragShade
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// ERROR HANDLING:
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	/* SHADER PROGRAM */
	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* INTERPRET VERTEX DATA */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* GENERATE VAO */
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// Initialization code:
	// Bind VAO:
	glBindVertexArray(VAO);
	// copy vertices array in a buffer for OpenGL to use:
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// copy index array element buffer for OpenGL:
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// set vertex attribute pointers:
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// WIREFRAME MODE:
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/* RENDER LOOP */
	while (!glfwWindowShouldClose(window))
	{
		// Calls input processor:
		ProcessInput(window);
		
		// Rendering Commands Here:
		// Draw Object:
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swaps the color buffer (contains color values for each pixel in GLFW Window
		glfwSwapBuffers(window);
		// Checks for triggered events (keyboard, mouse movements, etc.)
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

// Process All Input:
void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

/* GLFW: Whenever the window size changes, by the OS or the user, this function will execute: */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Adjust the size:
	glViewport(0, 0, width, height);
}