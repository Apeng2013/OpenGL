#include "TestPBR.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"
#include "Camera/Camera.h"

namespace Test {
	TestPBR::TestPBR()
		:m_Sphere(), m_Shader("res/Shaders/TestPBR/pbr.shader")
	{
	}

	TestPBR::~TestPBR()
	{
	}

	void TestPBR::OnUpdate(float delta_time)
	{
	}

	void TestPBR::OnRender()
	{
		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();
		Camera& camera = Camera::GetCamera();
		glm::mat4 view = camera.GetViewMatrix();

		m_Shader.Bind();
		m_Shader.SetUniformMat4f("uProj", projection);
		m_Shader.SetUniformMat4f("uView", view);
		m_Shader.SetUniform3f("uCameraPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		m_Shader.SetUniform3f("uAlbedo", 0.7f, 0.0f, 0.0f);
		m_Shader.SetUniform1f("uAo", 0.7f);

		m_Shader.SetUniform3f("uPointLights[0].LightPos", 0.0f, 0.0f, 0.0f);
		m_Shader.SetUniform3f("uPointLights[0].LightColor", 10.5f, 20.5f, 10.5f);

		m_Shader.SetUniform3f("uPointLights[1].LightPos", 10.0f, 10.0f, 5.0f);
		m_Shader.SetUniform3f("uPointLights[1].LightColor", 20.f, 20.f, 10.f);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.5f));
		for (int i = 0; i <= 10; i++)
		{
			m_Shader.SetUniform1f("uMetailic", float(i) / 10.0f);
			for (int j = 0; j <= 10; j++)
			{
				glm::mat4 cur_model = glm::translate(model, glm::vec3(i * 2.0, j * 2.0, 0));
				m_Shader.SetUniformMat4f("uModel", cur_model);
				m_Shader.SetUniform1f("uRoughness", float(j) / 10.0f);
				m_Sphere.Draw(m_Shader);
			}
		}


	}

	void TestPBR::OnImGuiRender()
	{
	}
}