#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	const char* glsl_version = "#version 330 core";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		return -1;

	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

	// Set up vertex data (and buffer(s)) and attribute pointers
	// use with Orthographic Projection
	/*
	GLfloat positions[] = {
		-0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 0.0f,
		0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 0.0f,
		0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
		0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
		-0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
		-0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 0.0f,

		-0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
		0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
		0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f,
		0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f,
		-0.5f * 500,  0.5f * 500,  0.5f * 500,  0.0f, 1.0f,
		-0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,

		-0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
		-0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
		-0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
		-0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
		-0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
		-0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,

		0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
		0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
		0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
		0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
		0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
		0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,

		-0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
		0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
		0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
		0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
		-0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
		-0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,

		-0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
		0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
		0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
		0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
		-0.5f * 500,  0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
		-0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f
	};*/
	float positions[] = 
	{
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	VertexArray va;
	VertexBuffer vb(positions, 6 * 5 * 6 * sizeof(float));
	
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);
	
	Shader shader("res/shaders/basic.shader");
	shader.Bind();
	
	Texture texture("res/textures/image.jpg");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	Renderer renderer;
	
	// Ortho
	//glm::mat4 projection = glm::ortho(0.0f,(float)screenWidth, 0.0f, (float)//screenHeight,0.10f,1000.0f); 
	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3/(screenWidth/2,screenHeight/2, /0));
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0, 0));
	
	//
	//shader.SetUniformMat4f("u_Model", model);
	//shader.SetUniformMat4f("u_View", view);
	//shader.SetUniformMat4f("u_Proj", projection);

	//Pers
	glm::mat4 projection;
	
	projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Poll for and process events */
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		/* Render here */
		renderer.Clear();
		glm::mat4 model;
		glm::mat4 view;
		
		model = glm::rotate(model, 10.0f, glm::vec3(0.5f,1.0f,0.0f));
		view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,-3.0f));
		
		shader.SetUniformMat4f("u_Model", model);
		shader.SetUniformMat4f("u_View", view);
		shader.SetUniformMat4f("u_Proj", projection);

		renderer.Draw(va, shader);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
	}


	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

