#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"


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

	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCALL(glEnable(GL_BLEND));

	//顶点数组
	VertexArray va;

	//顶点缓冲区
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	};
	VertexBuffer vb(vertices, 4 * 5 * sizeof(float));
	//顶点布局
	VertexBufferLayout vbl(
		{
			{GL_FLOAT, 3, 0, 0},
			{GL_FLOAT, 2, 0, 0}
		}
	);

	va.AddBuffer(vb, vbl);

	//索引缓冲区
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	IndexBuffer ib(indices, 2 * 3);

	// shader
	Shader shader("res/Shaders/base.shader");
	shader.Bind();

	Texture texture("res/Textures/letter_p.png");
	texture.Bind();

	shader.SetUniform1i("u_Texture", 0);

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