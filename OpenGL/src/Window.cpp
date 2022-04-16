#include "Window.h"
#include "Renderer.h"
#include <iostream>

Window::Window()
	:m_Window(nullptr), m_Width(0), m_Height(0)
{
}

Window::~Window()
{
}

void Window::Init(unsigned int width, unsigned int height, const std::string& name)
{
	if (!glfwInit())
	{
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Width = width;
	m_Height = height;
	m_Window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);

	if (!m_Window)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window);

	glfwSwapInterval(1);

	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCALL(glEnable(GL_BLEND));
}

void Window::Update() const
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

bool Window::ShouldCloseWindow() const
{
	return glfwWindowShouldClose(m_Window);
}

void Window::Close() const
{
	glfwTerminate();
}
