#include "TestIBL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"
#include "Camera/Camera.h"
#include "imgui.h"

namespace Test {
	TestIBL::TestIBL()
		:m_Sphere(), m_Shader("res/Shaders/TestPBR/pbr.shader"),
		m_Metallic(0.0f), m_Roughness(0.0f)
	{
	}

	TestIBL::~TestIBL()
	{
	}

	void TestIBL::OnUpdate(float delta_time)
	{
	}

	void TestIBL::OnRender()
	{
		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();
		Camera& camera = Camera::GetCamera();
		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.5f));

		m_Shader.Bind();
		m_Shader.SetUniformMat4f("uProj", projection);
		m_Shader.SetUniformMat4f("uView", view);
		m_Shader.SetUniformMat4f("uModel", model);
		m_Shader.SetUniform3f("uCameraPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		m_Shader.SetUniform3f("uAlbedo", 0.7f, 0.0f, 0.0f);
		m_Shader.SetUniform1f("uMetallic", m_Metallic);
		m_Shader.SetUniform1f("uRoughness", m_Roughness);
		m_Shader.SetUniform1f("uAo", 0.7f);

		m_Shader.SetUniform3f("uPointLights[0].LightPos", 0.0f, 0.0f, 0.0f);
		m_Shader.SetUniform3f("uPointLights[0].LightColor", 0.5f, 0.5f, 0.5f);

		m_Shader.SetUniform3f("uPointLights[1].LightPos", 0.0f, 2.0f, 0.0f);
		m_Shader.SetUniform3f("uPointLights[1].LightColor", 0.1f, 0.5f, 0.4f);

		m_Sphere.Draw(m_Shader);

	}

	void TestIBL::OnImGuiRender()
	{
		ImGui::SliderFloat("Metallic", &m_Metallic, 0.0f, 1.0f);
		ImGui::SliderFloat("Roughness", &m_Roughness, 0.0f, 1.0f);
	}
}