#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (unsigned int error = glGetError())
	{
		std::cout << "[OpenGL Error] " << error << " Function: " << function
			<< " File: " << file << " Line: " << line << std::endl;
		return false;
	}
	return true;
}

Renderer::Renderer()
{
	GLCALL(glEnable(GL_DEPTH_TEST));
}

Renderer::~Renderer()
{
}

void Renderer::Clear()
{
	GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCALL(glClearColor(0.3, 0.3, 0.4, 1.0));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	va.Bind();
	ib.Bind();
	shader.Bind();
	GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0));
}
