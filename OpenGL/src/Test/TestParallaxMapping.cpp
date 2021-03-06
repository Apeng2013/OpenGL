
#include "TestParallaxMapping.h"
#include "Window.h"
#include "Camera/Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"

namespace Test {
	TestParallaxMapping::TestParallaxMapping()
		:m_Wall(2.0f, 2.0f), m_Shader("res/Shaders/TestParallaxMapping/base.shader"),
		m_WallTex("res/Textures/bricks2.jpg"), m_NormalTex("res/Textures/bricks2_normal.jpg"),
		m_DepthTex("res/Textures/bricks2_disp.jpg")
	{

	}

	TestParallaxMapping::~TestParallaxMapping()
	{

	}
	void TestParallaxMapping::OnUpdate(float delta_time)
	{

	}

	void TestParallaxMapping::OnRender()
	{
		Camera& camera = Camera::GetCamera();
		glm::mat4 view = camera.GetViewMatrix();

		Window& window = Window::GetWindow();
		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

		glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
		glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
		glm::vec3 pos3(1.0f, -1.0f, 0.0f);

		glm::vec3 e1 = pos1 - pos3;
		glm::vec3 e2 = pos2 - pos3;

		glm::vec2 uv1(0.0f, 1.0f);
		glm::vec2 uv2(0.0f, 0.0f);
		glm::vec2 uv3(1.0f, 0.0f);

		glm::vec2 deltaUV1 = uv1 - uv3;
		glm::vec2 deltaUV2 = uv2 - uv3;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec4 tangent;
		tangent.x = f * (deltaUV2.y * e1.x - deltaUV1.y * e2.x);
		tangent.y = f * (deltaUV2.y * e1.y - deltaUV1.y * e2.y);
		tangent.z = f * (deltaUV2.y * e1.z - deltaUV1.y * e2.z);
		tangent.w = 0.0f;

		glm::vec4 bitangent;
		bitangent.x = f * (-deltaUV2.x * e1.x + deltaUV1.x * e2.x);
		bitangent.y = f * (-deltaUV2.x * e1.y + deltaUV1.x * e2.y);
		bitangent.z = f * (-deltaUV2.x * e1.z + deltaUV1.x * e2.z);
		bitangent.w = 0.0f;

		glm::vec4 normal(0.0f, 0.0f, 1.0f, 0.0f);

		glm::vec4 mm(0.0f, 0.0f, 0.0f, 1.0f);

		glm::mat4 tangent_mat(glm::normalize(tangent), glm::normalize(bitangent), glm::normalize(normal), glm::normalize(mm));

		m_Shader.Bind();
		m_WallTex.Bind();
		m_NormalTex.Bind(1);
		m_DepthTex.Bind(2);
		m_Shader.SetUniform1i("uTexture", 0);
		m_Shader.SetUniform1i("uNormalTex", 1);
		m_Shader.SetUniform1i("uDepthTex", 2);
		m_Shader.SetUniform1f("uDepthScale", 1.0f);

		m_Shader.SetUniformMat4f("uProj", projection);
		m_Shader.SetUniformMat4f("uView", view);
		m_Shader.SetUniformMat4f("uModel", model);
		m_Shader.SetUniform3f("uLightPos", 0.0f, 0.0f, 0.0f);
		m_Shader.SetUniform3f("uCameraPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		m_Shader.SetUniformMat4f("uTangentSpace", tangent_mat);

		m_Shader.SetUniform1i("uShinness", 32);
		m_Shader.SetUniform3f("uPointLight.position", 0.0f, 0.0f, 0.0f);
		m_Shader.SetUniform3f("uPointLight.ambient", 0.1f, 0.1f, 0.1f);
		m_Shader.SetUniform3f("uPointLight.diffuse", 0.5f, 0.5f, 0.5f);
		m_Shader.SetUniform3f("uPointLight.specular", 0.8f, 0.8f, 0.8f);
		m_Shader.SetUniform1f("uPointLight.constanct", 1.0f);
		m_Shader.SetUniform1f("uPointLight.linear", 0.07f);
		m_Shader.SetUniform1f("uPointLight.quadratic", 0.017f);

		m_Wall.Draw(m_Shader);
	}

	void TestParallaxMapping::OnImGuiRender()
	{

	}
}