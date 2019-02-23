#define _CRT_SECURE_NO_WARNINGS

#include <glew.h>
#include <glfw3.h>

#include "common_header.h"

#include "shaders.h"
#include "texture.h"
#include "vertexBufferObject.h"

#include "flyingCamera.h"

#include "freeTypeFont.h"

#include "skybox.h"
#include "dirLight.h"

#include "assimp_model.h"

#include "heightmap.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

unsigned int screen_width = 800;
unsigned int screen_height = 600;

float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;
bool firstMouse = true;

float currentFrame = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float fFrameInterval = float(deltaTime) / float(CLOCKS_PER_SEC);

float fAngleOfDarkness = 45.0f;

glm::vec3 wolfy = glm::vec3(30.0f, 100.0f, 10.0f);

CVertexBufferObject vboSceneObjects;
UINT uiVAOSceneObjects;

CFreeTypeFont ftFont;

//CSkybox sbMainSkybox;
Camera cCamera(glm::vec3(0.0f, 100.0f, 100.0f));

CDirectionalLight dlSun;

CAssimpModel amModels[3];

CMultiLayeredHeightmap hmWorld;

/*-----------------------------------------------

Name:    InitScene

Params:  lpParam - Pointer to anything you want.

Result:  Initializes OpenGL features that will
		 be used.

/*---------------------------------------------*/

void InitScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	if (!PrepareShaderPrograms())
	{
		std::cout << "Shader Program Failed" << std::endl;
		return;
	}

	LoadAllTextures();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	// Here we load font with pixel size 32 - this means that if we print with size above 32, the quality will be low
	ftFont.LoadSystemFont("arial.ttf", 32);
	ftFont.SetShaderProgram(&spFont2D);

	//cCamera = CFlyingCamera(glm::vec3(0.0f, 30.0f, 100.0f), glm::vec3(0.0f, 30.0f, 99.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f, 0.1f);
	//cCamera.SetMovingKeys('W', 'S', 'A', 'D');

	//sbMainSkybox.LoadSkybox("data\\skyboxes\\elbrus\\", "elbrus_front.jpg", "elbrus_back.jpg", "elbrus_right.jpg", "elbrus_left.jpg", "elbrus_top.jpg", "elbrus_top.jpg");

	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f) / 2, -sqrt(2.0f) / 2, 0), 0.5f);

	amModels[0].LoadModelFromFile((char*)"data\\models\\Wolf\\Wolf.obj");
	amModels[1].LoadModelFromFile((char*)"data\\models\\house\\house.3ds");
	CAssimpModel::FinalizeVBO();

	CMultiLayeredHeightmap::LoadTerrainShaderProgram();
	hmWorld.LoadHeightMapFromImage("data\\worlds\\consider_this_question.bmp");
}

/*-----------------------------------------------

Name:    RenderScene

Params:  lpParam - Pointer to anything you want.

Result:  Renders whole scene.

/*---------------------------------------------*/

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	spMain.UseProgram();

	// projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(cCamera.Zoom), (float)screen_width / (float)screen_height, 0.1f, 10000.0f);
	spMain.SetUniform("matrices.projMatrix", projection);
	spMain.SetUniform("matrices.viewMatrix", cCamera.GetViewMatrix());

	spMain.SetUniform("gSampler", 0);

	spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0));
	spMain.SetUniform("matrices.normalMatrix", glm::mat4(1.0));
	spMain.SetUniform("vColor", glm::vec4(1, 1, 1, 1));
	;
	// Set the directional vector of light
	dlSun.vDirection = glm::vec3(-sin(fAngleOfDarkness*3.1415f / 180.0f), -cos(fAngleOfDarkness*3.1415f / 180.0f), 0.0f);
	dlSun.SetUniformData(&spMain, "sunLight");

	spMain.SetUniform("matrices.modelMatrix", glm::translate(glm::mat4(1.0), cCamera.Position));
	//sbMainSkybox.RenderSkybox();

	spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0));

	// Now we're going to render terrain
	hmWorld.SetRenderSize(10000.0f, 100.0f, 10000.0f);
	// Render a house

	CAssimpModel::BindModelsVAO();

	glm::mat4 mModel = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, hmWorld.HeightAt(glm::vec3(0.0f,0.0f,0.0f)), 0.0f));
	mModel = glm::scale(mModel, glm::vec3(8, 8, 8)); // Casino :D

	spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);
	amModels[1].RenderModel();

	// ... and also ONE wolf now only :P
	//
	mModel = glm::translate(glm::mat4(1.0), glm::vec3(-30.0f, hmWorld.HeightAt(glm::vec3(-30.0f, 0.0f, 0.0f)), 0.0f));
	mModel = glm::scale(mModel, glm::vec3(3.0f, 3.0f, 3.0f));

	spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModel);
	amModels[0].RenderModel();



	CShaderProgram* spTerrain = CMultiLayeredHeightmap::GetShaderProgram();

	spTerrain->UseProgram();

	projection = glm::perspective(glm::radians(cCamera.Zoom), (float)screen_width / (float)screen_height, 0.1f, 10000.0f);
	spTerrain->SetUniform("matrices.projMatrix", projection);
	spTerrain->SetUniform("matrices.viewMatrix", cCamera.GetViewMatrix());

	// We bind all 5 textures - 3 of them are textures for layers, 1 texture is a "path" texture, and last one is
	// the places in heightmap where path should be and how intense should it be
	FOR(i, 5)
	{
		char sSamplerName[256];
		sprintf_s(sSamplerName, "gSampler[%d]", i);
		tTextures[i].BindTexture(i);
		spTerrain->SetUniform(sSamplerName, i);
	}

	// ... set some uniforms
	spTerrain->SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", glm::mat4(1.0));
	spTerrain->SetUniform("vColor", glm::vec4(1, 1, 1, 1));

	dlSun.SetUniformData(spTerrain, "sunLight");

	// ... and finally render heightmap
	hmWorld.RenderHeightmap();

	cCamera.updateCameraVectors();

	// Print something over scene
			
	spFont2D.UseProgram();
	glDisable(GL_DEPTH_TEST);
	// projection matrix
	projection = glm::perspective(glm::radians(cCamera.Zoom), (float)screen_width / (float)screen_height, 0.1f, 1000.0f);
	spFont2D.SetUniform("matrices.projMatrix", projection);

	spFont2D.SetUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	ftFont.Print("Create By - P.G.", 20, 20, 24);

	glEnable(GL_DEPTH_TEST);
	//if(Keys::Onekey(VK_ESCAPE))PostQuitMessage(0);

	//oglControl->SwapBuffers();
}

/*-----------------------------------------------

Name:    ReleaseScene

Params:  lpParam - Pointer to anything you want.

Result:  Releases OpenGL scene.

/*---------------------------------------------*/

void ReleaseScene()
{
	FOR(i, NUMTEXTURES)tTextures[i].DeleteTexture();
	//sbMainSkybox.DeleteSkybox();

	spMain.DeleteProgram();
	spOrtho2D.DeleteProgram();
	spFont2D.DeleteProgram();
	FOR(i, NUMSHADERS)shShaders[i].DeleteShader();
	ftFont.DeleteFont();

	glDeleteVertexArrays(1, &uiVAOSceneObjects);
	vboSceneObjects.DeleteVBO();

	hmWorld.ReleaseHeightmap();
	CMultiLayeredHeightmap::ReleaseTerrainShaderProgram();
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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	InitScene();
	
	while (!glfwWindowShouldClose(window)) {
		// per frame logic
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		fAngleOfDarkness += deltaTime;
		// input
		processInput(window);
		
		RenderScene();
		
		// swap buffers and poll IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ReleaseScene();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cCamera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cCamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cCamera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
		fAngleOfDarkness += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
	{
		fAngleOfDarkness -= -0.01f;
	}
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

	cCamera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cCamera.ProcessMouseScroll(yoffset);
}	