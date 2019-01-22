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


glm::mat4 glPerspective(const float angle, const float ar, const float zNear, const float zFar)
{
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(angle / 2.0);
	
	glm::mat4 m(1.0f);

	m.m[0][0] = 1.0f / (tanHalfFOV * ar);
	m.m[0][1] = 0.0f;
	m.m[0][2] = 0.0f;
	m.m[0][3] = 0.0f;

	m.m[1][0] = 0.0f;
	m.m[1][1] = 1.0f / tanHalfFOV;
	m.m[1][2] = 0.0f;
	m.m[1][3] = 0.0f;

	m.m[2][0] = 0.0f;
	m.m[2][1] = 0.0f;
	m.m[2][2] = (-zNear - zFar) / zRange;
	m.m[2][3] = 2.0f * zFar * zNear / zRange;

	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = 1.0f;
	m.m[3][3] = 0.0f;
	
	return m;
}


glm::mat4 glOrthographics(const float &b, const float &t, const float &l, const float &r, const float &n, const float &f)
{
	glm::mat4 M(1.0f);
	// set OpenGL perspective projection matrix
	M[0][0] = 2 / (r - l);
	M[0][1] = 0;
	M[0][2] = 0;
	M[0][3] = 0;

	M[1][0] = 0;
	M[1][1] = 2 / (t - b);
	M[1][2] = 0;
	M[1][3] = 0;

	M[2][0] = 0;
	M[2][1] = 0;
	M[2][2] = -2 / (f - n);
	M[2][3] = 0;

	M[3][0] = -(r + l) / (r - l);
	M[3][1] = -(t + b) / (t - b);
	M[3][2] = -(f + n) / (f - n);
	M[3][3] = 1;

	return M;
}


glm::mat4 glRotate(float angle, glm::vec3 const& v)
{
	float const a = angle;
	float const c = cos(a);
	float const s = sin(a);

	glm::mat4 M(1.0f);
	M[0][0] = c + v.x*v.x*(1 - c);
	M[0][1] = v.x*v.y*(1 - c) - v.z*s;
	M[0][2] = v.x*v.z*(1 - c) + v.y*s;
	M[0][3] = 0;

	M[1][0] = v.x*v.y*(1 - c) + v.z*s;
	M[1][1] = c + v.y*v.y*(1 - c);
	M[1][2] = v.y*v.z*(1 - c) - v.x*s;
	M[1][3] = 0;

	M[2][0] = v.z*v.x*(1 - c) - v.y*s;
	M[2][1] = v.z*v.y*(1 - c) + v.x*s;
	M[2][2] = c + v.z*v.z*(1 - c);
	M[2][3] = 0;

	M[3][0] = 0;
	M[3][1] = 0;
	M[3][2] = 0;
	M[3][3] = 1;

	return M;
}

glm::mat4 glTranslate(glm::mat4 const& m, glm::vec3 const& v)
{
	glm::mat4 r(m);
	r[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
	return r;
}

int main(void)
{

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	const char* glsl_version = "#version 330 core";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	GLFWwindow* window;
	
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 660, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	//glfwSwapInterval(1);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			return -1;

	glEnable(GL_DEPTH_TEST);

	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	//OrthoGraphic
	
	GLfloat positions[] = {
		-0.5f *200, -0.5f *200, -0.5f *200,  0.0f, 0.0f,   
		0.5f *200, -0.5f *200, -0.5f *200,  1.0f, 0.0f,  	
		0.5f *200,  0.5f *200, -0.5f *200,  1.0f, 1.0f,  	
		0.5f *200,  0.5f *200, -0.5f *200,  1.0f, 1.0f,  	
		-0.5f *200,  0.5f *200, -0.5f *200,  0.0f, 1.0f, 	
		-0.5f *200, -0.5f *200, -0.5f *200,  0.0f, 0.0f, 	

		-0.5f *200, -0.5f *200,  0.5f *200,  0.0f, 0.0f, 	
		0.5f *200, -0.5f *200,  0.5f *200,  1.0f, 0.0f,  	
		0.5f *200,  0.5f *200,  0.5f *200,  1.0f, 1.0f,  	
		0.5f *200,  0.5f *200,  0.5f *200,  1.0f, 1.0f,  	
		-0.5f *200,  0.5f *200,  0.5f *200,  0.0f, 1.0f, 	
		-0.5f *200, -0.5f *200,  0.5f *200,  0.0f, 0.0f, 	

		-0.5f *200,  0.5f *200,  0.5f *200,  1.0f, 0.0f, 	
		-0.5f *200,  0.5f *200, -0.5f *200,  1.0f, 1.0f, 	
		-0.5f *200, -0.5f *200, -0.5f *200,  0.0f, 1.0f, 	
		-0.5f *200, -0.5f *200, -0.5f *200,  0.0f, 1.0f, 	
		-0.5f *200, -0.5f *200,  0.5f *200,  0.0f, 0.0f, 	
		-0.5f *200,  0.5f *200,  0.5f *200,  1.0f, 0.0f, 	

		0.5f *200,  0.5f *200,  0.5f *200,  1.0f, 0.0f,  	
		0.5f *200,  0.5f *200, -0.5f *200,  1.0f, 1.0f,  	
		0.5f *200, -0.5f *200, -0.5f *200,  0.0f, 1.0f,  	
		0.5f *200, -0.5f *200, -0.5f *200,  0.0f, 1.0f,  	
		0.5f *200, -0.5f *200,  0.5f *200,  0.0f, 0.0f,  	
		0.5f *200,  0.5f *200,  0.5f *200,  1.0f, 0.0f,  	

		-0.5f *200, -0.5f *200, -0.5f *200,  0.0f, 1.0f, 	
		0.5f *200, -0.5f *200, -0.5f *200,  1.0f, 1.0f,  	
		0.5f *200, -0.5f *200,  0.5f *200,  1.0f, 0.0f,  	
		0.5f *200, -0.5f *200,  0.5f *200,  1.0f, 0.0f,  	
		-0.5f *200, -0.5f *200,  0.5f *200,  0.0f, 0.0f, 	
		-0.5f *200, -0.5f *200, -0.5f *200,  0.0f, 1.0f, 	

		-0.5f *200,  0.5f *200, -0.5f *200,  0.0f, 1.0f, 	
		0.5f *200,  0.5f *200, -0.5f *200,  1.0f, 1.0f,  	
		0.5f *200,  0.5f *200,  0.5f *200,  1.0f, 0.0f,  	
		0.5f *200,  0.5f *200,  0.5f *200,  1.0f, 0.0f,  	
		-0.5f *200,  0.5f *200,  0.5f *200,  0.0f, 0.0f, 	
		-0.5f *200,  0.5f *200, -0.5f *200,  0.0f, 1.0f 
	};
	
	//Perspective
	/*
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
	};*/
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	//glDepthMask(GL_TRUE);
	//glDepthFunc(GL_LESS);

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
	
	//Perspective
	//glm::mat4 projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);
	//glm::mat4 model = glm::rotate(glm::mat4(1.0f), -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.50f, -3.0f));
	
	//Orthographic
	//glm::mat4 projection = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, 0.1f, 100.0f);
	//glm::mat4 model = glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection = glOrthographics(0.0f, (float)screenHeight, 0.0f, (float)screenWidth, 0.1f, 1000.0f);
	glm::mat4 view = glTranslate(glm::mat4(1.0f), glm::vec3((float)screenWidth/2, (float)screenHeight/2, 0.0f));
	glm::mat4 model = glRotate(90.0f, glm::vec3(0.0f, 0.0f, 0.0f));

	//float b, t, l, r;
	//glPerspectiveVar(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f, b, t, l, r);
	//glm::mat4 projection = glPerspective(b, t, l, r, 0.1f, 100.0f);
	//glm::mat4 view = glTranslate(glm::mat4(1.0f), glm::vec3(0.0f,0.50f, -2.0f));
	//glm::mat4 model = glRotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	shader.SetUniformMat4f("u_Proj", projection);
	shader.SetUniformMat4f("u_View", view);
	shader.SetUniformMat4f("u_Model", model);

	while (!glfwWindowShouldClose(window))
	{
		/* Poll for and process events */
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		/* Render here */
		renderer.Clear();

		glm::mat4 model = glRotate((float)glfwGetTime() * -1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * -1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		shader.SetUniformMat4f("u_Model", model);

		renderer.Draw(va, shader);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);		
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

