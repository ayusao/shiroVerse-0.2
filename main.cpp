#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#define windowx 800
#define windowy 800
int main()
{
	glfwInit();
	// Tell GLFW what version of OpenGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowx, windowy, "Window yayyy!", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introducing the window into the current context
	glfwMakeContextCurrent(window);
	gladLoadGL();

	glViewport(0, 0, windowx, windowy);
	// Specifying the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);
	while (!glfwWindowShouldClose(window))
	{
		// Take care of all GLFW events
		glfwPollEvents();
	}
	// Delete window before ending the program
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}