#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "texture.h"

#include <iostream>

float x = 1.0f, y = 1.0f, z = 1.0f, angle = 0.0f, cam_depth = -300.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	angle = 0.0005f;
	z = 0.0f;
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		y = -1.0f;
		x = 0.0f;
		//angle-=0.001f;
	//	std::cout << angle << std::endl;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		y = 1.0f;
		x = 0.0f;
		//angle+=0.001f;
	//	std::cout << angle << std::endl;
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		x = -1.0f;
		y = 0.0f;
		//angle-=0.001f;
	//	std::cout << angle << std::endl;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		x = 1.0f;
		y = 0.0f;
		//angle+=0.001f;
	//	std::cout << angle << std::endl;
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		cam_depth+=10;
		//angle+=0.001f;
		//std::cout << cam_depth << std::endl;
	}
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		cam_depth-=10;
		//angle+=0.001f;
		//std::cout << cam_depth << std::endl;
	}
}

glm::mat4 glPerspective(const float angle, const float ar, const float zNear, const float zFar)
{
	const float zRange = zFar-zNear;
	const float tanHalfFOV = tan(angle / 2.0);

	glm::mat4 m(0.0f);

	m[0][0] = 1.0f / (tanHalfFOV * ar);
	m[1][1] = 1.0f / tanHalfFOV;
	m[2][2] = zFar / zRange;
	m[2][3] = 1.0f;
	m[3][2] = -(zFar * zNear) / zRange;

	return m;
}


int main(void) 
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window;
	window = glfwCreateWindow(960,540, "Computer Graphics", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		return -1;

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
		
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	
	std::cout << "Use\n0 to zoom out\n1 to zoom in\narrows to rotate about x and y axis" << std::endl;

	const char* glsl_version = "#version 330 core";

	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	GLfloat positions[] = {
		-0.5f * 200, -0.5f * 200, -0.5f * 200,  0.0f, 0.0f,
		0.5f * 200, -0.5f * 200, -0.5f * 200,  1.0f, 0.0f,
		0.5f * 200,  0.5f * 200, -0.5f * 200,  1.0f, 1.0f,
		0.5f * 200,  0.5f * 200, -0.5f * 200,  1.0f, 1.0f,
		-0.5f * 200,  0.5f * 200, -0.5f * 200,  0.0f, 1.0f,
		-0.5f * 200, -0.5f * 200, -0.5f * 200,  0.0f, 0.0f,

		-0.5f * 200, -0.5f * 200,  0.5f * 200,  0.0f, 0.0f,
		0.5f * 200, -0.5f * 200,  0.5f * 200,  1.0f, 0.0f,
		0.5f * 200,  0.5f * 200,  0.5f * 200,  1.0f, 1.0f,
		0.5f * 200,  0.5f * 200,  0.5f * 200,  1.0f, 1.0f,
		-0.5f * 200,  0.5f * 200,  0.5f * 200,  0.0f, 1.0f,
		-0.5f * 200, -0.5f * 200,  0.5f * 200,  0.0f, 0.0f,

		-0.5f * 200,  0.5f * 200,  0.5f * 200,  1.0f, 0.0f,
		-0.5f * 200,  0.5f * 200, -0.5f * 200,  1.0f, 1.0f,
		-0.5f * 200, -0.5f * 200, -0.5f * 200,  0.0f, 1.0f,
		-0.5f * 200, -0.5f * 200, -0.5f * 200,  0.0f, 1.0f,
		-0.5f * 200, -0.5f * 200,  0.5f * 200,  0.0f, 0.0f,
		-0.5f * 200,  0.5f * 200,  0.5f * 200,  1.0f, 0.0f,

		0.5f * 200,  0.5f * 200,  0.5f * 200,  1.0f, 0.0f,
		0.5f * 200,  0.5f * 200, -0.5f * 200,  1.0f, 1.0f,
		0.5f * 200, -0.5f * 200, -0.5f * 200,  0.0f, 1.0f,
		0.5f * 200, -0.5f * 200, -0.5f * 200,  0.0f, 1.0f,
		0.5f * 200, -0.5f * 200,  0.5f * 200,  0.0f, 0.0f,
		0.5f * 200,  0.5f * 200,  0.5f * 200,  1.0f, 0.0f,

		-0.5f * 200, -0.5f * 200, -0.5f * 200,  0.0f, 1.0f,
		0.5f * 200, -0.5f * 200, -0.5f * 200,  1.0f, 1.0f,
		0.5f * 200, -0.5f * 200,  0.5f * 200,  1.0f, 0.0f,
		0.5f * 200, -0.5f * 200,  0.5f * 200,  1.0f, 0.0f,
		-0.5f * 200, -0.5f * 200,  0.5f * 200,  0.0f, 0.0f,
		-0.5f * 200, -0.5f * 200, -0.5f * 200,  0.0f, 1.0f,

		-0.5f * 200,  0.5f * 200, -0.5f * 200,  0.0f, 1.0f,
		0.5f * 200,  0.5f * 200, -0.5f * 200,  1.0f, 1.0f,
		0.5f * 200,  0.5f * 200,  0.5f * 200,  1.0f, 0.0f,
		0.5f * 200,  0.5f * 200,  0.5f * 200,  1.0f, 0.0f,
		-0.5f * 200,  0.5f * 200,  0.5f * 200,  0.0f, 0.0f,
		-0.5f * 200,  0.5f * 200, -0.5f * 200,  0.0f, 1.0f
	};

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

	glm::mat4 projection, view, model;

	projection = glPerspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 1000.0f);
	projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 1000.0f);
	view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -100.0f));
	model = glm::scale(glm::mat4(1.0f),glm::vec3(0.2f, 0.2f, 0.2f));

	shader.SetUniformMat4f("u_Proj", projection);
	shader.SetUniformMat4f("u_View", view);
	shader.SetUniformMat4f("u_Model", model);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		renderer.Clear();

		model = glm::rotate(model, angle, glm::vec3(x, y, z));
		shader.SetUniformMat4f("u_Model", model);

		view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, cam_depth));
		shader.SetUniformMat4f("u_View", view);

		renderer.Draw(va, shader);
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}