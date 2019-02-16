#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "texture.h"
#include "camera.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Settings
unsigned int screen_width = 800;
unsigned int screen_height = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;
bool firstMouse = true;

// Timing
float currentFrame = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Light
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 lightCol(1.0f, 1.0f, 1.0f);

int main() {

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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	Shader shaderObj("res/shaders/basic.shader");
	shaderObj.Unbind();
	Shader shaderLig("res/shaders/lamp.shader");
	shaderLig.Unbind();

	float vertices[] = {
		// Position [3] Texture Coordinates [2] Normals [3]
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,	0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,	0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,	0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,	0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,
									 
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,	0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,	0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,	0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,	0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,	0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,	0.0f,  0.0f,  1.0f,
									 
		-0.5f,  0.5f,  0.5f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
									 
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,	1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,	1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  1.0f,	1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  1.0f,	1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,	1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,	1.0f,  0.0f,  0.0f,
         					  		 
		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  1.0f,	0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,	0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,	0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,	0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,
							  		 
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
 		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,	0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f
	};

	float lightvertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Cubes
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	VertexArray va;
	VertexBuffer vb(vertices, 6 * 8 * 6 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);

	va.AddBuffer(vb, layout);

	Texture texture("res/textures/container.png");
	texture.Bind();

	shaderObj.Bind();
	shaderObj.SetUniform1i("u_Texture", 0);
	shaderObj.SetUniform3f("u_LightCol", lightCol.x, lightCol.y, lightCol.z);
	shaderObj.SetUniform3f("u_LightPos", lightPos.x, lightPos.y, lightPos.z);
	shaderObj.SetUniform3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
	shaderObj.SetUniform1f("u_LightcutOff", glm::cos(glm::radians(12.5f)));
	shaderObj.SetUniform1f("u_LightouterCutOff", glm::cos(glm::radians(17.5f)));

	shaderLig.Bind();
	shaderLig.SetUniform3f("u_LightCol", lightCol.x, lightCol.y, lightCol.z);

	// Light
	unsigned int lightvao;
	glGenVertexArrays(1, &lightvao);
	glBindVertexArray(lightvao);

	VertexArray lightva;
	VertexBuffer lightvb(lightvertices, 6 * 5 * 3 * sizeof(float));

	VertexBufferLayout lightlayout;
	lightlayout.Push<float>(3);

	lightva.AddBuffer(lightvb, lightlayout);

	Renderer renderer;

	while (!glfwWindowShouldClose(window)) {
		
		// per frame logic
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// render
		renderer.Clear();

		// projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
		shaderObj.Bind();
		shaderObj.SetUniformMat4f("u_Proj", projection);

		// camera / view transformation
		glm::mat4 view = camera.GetViewMatrix();
		shaderObj.SetUniformMat4f("u_View", view);

		//shaderObj.SetUniform3f("u_LightPos", lightPos.x, lightPos.y, lightPos.z);
		shaderObj.SetUniform3f("u_LightPos", camera.Position.x, camera.Position.y, camera.Position.z);
		shaderObj.SetUniform3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		shaderObj.SetUniform3f("u_LightDir", camera.Front.x, camera.Front.y, camera.Front.z);

		// render boxes
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shaderObj.SetUniformMat4f("u_Model", model);
			renderer.Draw(va, shaderObj);
		}

		shaderLig.Bind();
		shaderLig.SetUniformMat4f("u_Proj", projection);
		shaderLig.SetUniformMat4f("u_View", view);

		lightPos.x = 5.0f * sin(currentFrame * 0.1f);
		lightPos.y = 5.0f * cos(currentFrame * 0.1f);

		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.5f)); // a smaller cube
		//shaderLig.SetUniformMat4f("u_Model", model);
		//renderer.Draw(lightva, shaderLig);

		// swap buffers and poll IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
