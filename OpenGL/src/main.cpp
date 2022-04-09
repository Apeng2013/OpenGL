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

	float r = 0.0f;
	float interval = 0.05f;

	Renderer renderer;


	while (!glfwWindowShouldClose(window))
	{
		renderer.Clear();

		shader.Bind();
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

		renderer.Draw(va, ib, shader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}