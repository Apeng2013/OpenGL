#include "Renderer.h"
#include "Window.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera/Camera.h"
#include "Model/Model.h"
#include "Geometry/Cube.h"


static int width = 960, height = 720;
Camera camera(width, height);

void OnCursorPosChange(GLFWwindow* window, double xpos, double ypos)
{
    camera.OnCursorPosChange(xpos, ypos);
}

void OnKeyStateChange(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    camera.OnKeyStateChange(key, scancode, action, mods);
}


int main()
{
	Window& window = Window::GetWindow();
	window.Init(width, height, "OpenGL Window");

    Renderer& renderer = Renderer::GetRenderer();

    glfwSetCursorPosCallback(window.GetGLWindow(), OnCursorPosChange);
    glfwSetKeyCallback(window.GetGLWindow(), OnKeyStateChange);

    glfwSetInputMode(window.GetGLWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);

	//Model model("res/Models/Nanosuit/nanosuit.obj");
	//Shader shader("res/Shaders/man.shader");

    glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    glm::mat4 m1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    m1 = glm::scale(m1, glm::vec3(1.2f, 1.2f, 1.2f));

	Cube cube;
	Shader shader("res/Shaders/base.shader");
	Shader shader_edge("res/Shaders/cube_stencil.shader");
	Texture texture("res/Textures/container.png");

	while (!window.ShouldCloseWindow())
	{
		renderer.Clear();
		glm::mat4 view = camera.GetViewMatrix();

		GLCALL(glEnable(GL_DEPTH_TEST));
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xff);
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		shader.Bind();
		texture.Bind();
		shader.SetUniform1i("uTexture", 0);
		shader.SetUniformMat4f("uProj", projection);
		shader.SetUniformMat4f("uView", view);
		shader.SetUniformMat4f("uModel", m);
 		cube.Draw(shader);

		glDisable(GL_DEPTH_TEST);
		glStencilMask(0x00);
		glStencilFunc(GL_NOTEQUAL, 1, 0xff);
		shader_edge.Bind();
		shader_edge.SetUniformMat4f("uProj", projection);
		shader_edge.SetUniformMat4f("uView", view);
		shader_edge.SetUniformMat4f("uModel", m1);
		cube.Draw(shader_edge);

		glEnable(GL_DEPTH_TEST);
		glStencilMask(0xff);

		window.Update();
	}

	window.Close();

	return 0;
}