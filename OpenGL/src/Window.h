#pragma once

#include "GLFW/glfw3.h"
#include <string>


class Window
{
private:
	Window();
	~Window();
public:
	void Init(unsigned int width, unsigned int height, const std::string& name);
	void Update() const;
	bool ShouldCloseWindow() const;
	void Close() const;

	static Window& GetWindow() { return *s_WindowInstance; }
	static GLFWwindow* GetGLWindow() { return s_WindowInstance->GetGLWindowImpl(); }
	GLFWwindow* GetGLWindowImpl() const { return m_Window; }

public:
	Window(Window& window) = delete;
	Window& operator=(Window& window) = delete;

private:
	static Window* s_WindowInstance;
	GLFWwindow* m_Window;
	unsigned int m_Width, m_Height;
};