
#include "TestPostProcess.h"
#include "Window.h"
#include "Camera/Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"

namespace Test {
	TestPostProcess::TestPostProcess()
		:m_Cube(), m_Shader("res/Shaders/TestPostProcess/base.shader"),
		m_CubeTex("res/Textures/container.png")
	{

		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}

	TestPostProcess::~TestPostProcess()
	{

	}
	void TestPostProcess::OnUpdate(float delta_time)
	{

	}

	void TestPostProcess::OnRender()
	{
		Camera& camera = Camera::GetCamera();
		glm::mat4 view = camera.GetViewMatrix();

		Window& window = Window::GetWindow();
		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

		m_Shader.Bind();
		m_CubeTex.Bind();
		m_Shader.SetUniform1i("uTexture", 0);
		m_Shader.SetUniformMat4f("uProj", projection);
		m_Shader.SetUniformMat4f("uView", view);
		m_Shader.SetUniformMat4f("uModel", model);
		m_Cube.Draw(m_Shader);
	}

	void TestPostProcess::OnImGuiRender()
	{

	}
}