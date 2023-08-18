#include<iostream>
#include<glad/glad.h>

#include<GLFW/glfw3.h>

#include "game.h"
#include "resourceManager.h"
const unsigned int windowx = 1200;
const unsigned int windowy = 900;
//GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

Game alienVerse(windowx, windowy);


int main(int argc, char* argv[])
{
	glfwInit();
	// Tell GLFW what version of OpenGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, false);

	GLFWwindow* window = glfwCreateWindow(windowx, windowy, "SpaceEdition", NULL, NULL);
	// Introducing the window into the current context
	glfwMakeContextCurrent(window);

	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//initialize GLAD before we call any OpenGL function because GLAD manages function pointers for OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << " Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glViewport(0, 0, windowx, windowy);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//initializing the game
	alienVerse.Init();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	//the render loop
	while (!glfwWindowShouldClose(window))
	{
		//calculate delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Take care of all GLFW events
		glfwPollEvents();


		//keyboard input
		alienVerse.ProcessInput(deltaTime);

		//update game status
		alienVerse.Update(deltaTime);
		
			// Specifying the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		alienVerse.Render();

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
	}
	// Delete window before ending the program
	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
		if (key >= 0 && key < 1024) {
			if (action == GLFW_PRESS)
				alienVerse.Keys[key] = true;
			else if (action == GLFW_RELEASE)
			{
				alienVerse.Keys[key] = false;
				alienVerse.KeysProcessed[key] = false;
			}
		}

}

//void processInput(GLFWwindow* window) {
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}