#include "TestTexture2D.h"
#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"
#include "Renderer.h"
#include "imgui.h"
#include "Camera/Camera.h"

namespace Test {
	TestTexture2D::TestTexture2D()
	{
		//顶点数组
		m_VertexArray = std::make_unique<VertexArray>();

		//顶点缓冲区
		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		};
		m_VertexBuffer.reset(new VertexBuffer(vertices, 4 * 5 * sizeof(float)));

		//顶点布局
		m_VertexBufferLayout.reset( new VertexBufferLayout(
			{
				{GL_FLOAT, 3, 0, 0},{GL_FLOAT, 2, 0, 0}
			}
		));
		m_VertexArray->AddBuffer(*m_VertexBuffer, *m_VertexBufferLayout);

		//索引缓冲区
		unsigned int indices[] = {
			0, 1, 2,
			1, 3, 2
		};
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 2 * 3);

		// shader
		m_Shader = std::make_unique<Shader>("res/Shaders/TestTexture2D/TestTexture2D.shader");
		m_Shader->Bind();

		m_Texture = std::make_unique<Texture>("res/Textures/letter_p.png");
		m_Texture->Bind();

	}
	TestTexture2D::~TestTexture2D()
	{
	}
	void TestTexture2D::OnUpdate(float delta_time)
	{
	}
	void TestTexture2D::OnRender()
	{
		m_Shader->Bind();
		m_Texture->Bind();
		m_Shader->SetUniform1i("u_Texture", 0);

		Renderer::GetRenderer().Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
	}
	void TestTexture2D::OnImGuiRender()
	{
		//ImGui::SliderFloat2("position1(x,y)", m_Position1, 0.0f, 100.0f);
		//ImGui::SliderFloat2("position2(x,y)", m_Position2, 0.0f, 100.0f);
	}
}