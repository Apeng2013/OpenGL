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
    GLCALL(glEnable(GL_DEPTH_TEST));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);

	Model model("res/Models/Nanosuit/nanosuit.obj");

	Shader shader("res/Shaders/man.shader");

	while (!window.ShouldCloseWindow())
	{
		renderer.Clear();
        glm::mat4 m = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
        m = glm::translate(m, glm::vec3(0.0f, -10.0f, -10.0f));
        glm::mat4 mvp = projection * camera.GetViewMatrix() * m;
		shader.Bind();
		shader.SetUniformMat4f("uProj", projection);
		glm::mat4 view = camera.GetViewMatrix();
		shader.SetUniformMat4f("uView", view);
		shader.SetUniformMat4f("uModel", m);
		model.Draw(shader);

		window.Update();
	}

	window.Close();

	return 0;
}