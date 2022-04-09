#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"


int main()
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(780, 480, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	//顶点数组
	VertexArray va;

	//顶点缓冲区
	float vertices[] = {
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	VertexBuffer vb(vertices, 3 * 3 * sizeof(float));
	//顶点布局
	VertexBufferLayout vbl(
		{
			{GL_FLOAT, 3, 1, 0}
		}
	);

	va.AddBuffer(vb, vbl);

	//索引缓冲区
	unsigned int indices[] = {
		0, 1, 2
	};
	IndexBuffer ib(indices, 3);

	// shader
	Shader shader("../res/Shaders/triangle_shader.shader");
	shader.Bind();

	float r = 0.0f;
	float interval = 0.05f;

	va.Unbind();
	ib.Unbind();

	while (!glfwWindowShouldClose(window))
	{
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
		GLCALL(glClearColor(0.1f, 0.1f, 0.2f, 1.0f));

		shader.SetUniform4f("u_Color", r, 0.2f, 0.3f, 1.0f);
		if (r > 1.0f)
		{
			interval = -0.05f;
		}
		else if (r < 0.0f)
		{
			interval = 0.05f;
		}
		r += interval;

		va.Bind();
		ib.Bind();

		GLCALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}