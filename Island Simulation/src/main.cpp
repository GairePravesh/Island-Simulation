#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>

#include <glm.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

unsigned int screen_width = 800;
unsigned int screen_height = 600;

const int Rows = 100;
const int Cols = 100;

glm::vec3 TerrainHeights[Rows][Cols] = { glm::vec3(0.0f) };

void GenTerrainBuffer()
{
	float x = -1.0f, z = 1.0f;
	float xOff = 2.0f / (float)Rows;
	float zOff = 2.0f / (float)Cols;

	srand(time(NULL));

	for (int i = 0; i < Rows; i++)
	{
		x += xOff;
		for (int j = 0; j < Cols; j++)
		{
			TerrainHeights[i][j] = glm::vec3(x, -1.0f + ((float)rand() * 2.0f) / RAND_MAX , z);
			z += zOff;
		}
		z = -1.0f;
	}
}


void RenderTerrain()
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TerrainHeights), &TerrainHeights[0][0], GL_STATIC_DRAW);

	const char* vertexShader= "\n"
			"#version 330\n"
			"in vec3 position; \n"
			"void main()\n"
			"{\n"
				"gl_Position = vec4(position, 1.0); \n"
			"}\n";
	const char* fragmentShader = "\n"
			"#version 330\n"
			"out vec4 outColor; \n"
			"void main()\n"
			"{\n"
				"outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); \n"
			"}\n";

	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShader, NULL);
	glCompileShader(vertexShaderID);
	
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShader, NULL);
	glCompileShader(fragmentShaderID);

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderID);
	glAttachShader(shaderProgram, fragmentShaderID);	
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	unsigned int posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(posAttrib);
	
	int vertexCompilation;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &vertexCompilation);

	if (vertexCompilation == GL_FALSE)
	{
		std::cout << "Vertex SHader failed" << std::endl;
		int InfoLogLength = 0;
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0)
		{
			char vertexError[1000];
			glGetShaderInfoLog(vertexShaderID, InfoLogLength, NULL,
				&vertexError[0]);
			std::cout << &vertexError[0];
		}
	}
	
	int fragmentCompilation;
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &fragmentCompilation);

	if (fragmentCompilation == GL_FALSE)
	{
		std::cout << "fragment SHader failed" << std::endl;
	}
}

int main()
{
	// glfw: Initialize and configuration
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	//GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	//const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	//
	//glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	//glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	//glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	//glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	//
	//GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Graphics Project", monitor, NULL);
	//
	//screen_height = mode->height;
	//screen_width = mode->width;

	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Graphics Project", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	//glEnable(GL_DEPTH_TEST);

	GenTerrainBuffer();
	RenderTerrain();

	int size = sizeof(TerrainHeights) / sizeof(glm::vec3);

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, size);

		// swap buffers and poll IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
