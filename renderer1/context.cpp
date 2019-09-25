#include "context.h"

// system headers
#include <iostream>

// library headers
#include "glew/GL/glew.h"
#include "glfw/glfw3.h"

bool context::init(int width, int height, const char * title)
{
	glfwInit();

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewInit();

	std::cout << "OpenGL Version: " << (const char *)glGetString(GL_VERSION) << "\n";
	std::cout << "Renderer: " << (const char *)glGetString(GL_RENDERER) << "\n";
	std::cout << "Vendor: " << (const char *)glGetString(GL_VENDOR) << "\n";
	std::cout << "GLSL: " << (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	return true;
}

void context::tick()
{
	glfwPollEvents();
	glfwSwapBuffers(window);
}

void context::term()
{
	glfwDestroyWindow(window);
	glfwTerminate();

	window = nullptr;
}

bool context::shouldClose()const
{
	return glfwWindowShouldClose(window);
}



void context::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}