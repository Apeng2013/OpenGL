#include "TestTexture2D.h"
#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"
#include "Renderer.h"
#include "imgui.h"

namespace Test {
	TestTexture2D::TestTexture2D()
		:m_Proj(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Model(glm::mat4(1.0f)),
		m_Position1{ 0.0f, 0.0f }, m_Position2{0.0f, 0.0f}
	{
		//顶点数组
		m_VertexArray = std::make_unique<VertexArray>();

		//顶点缓冲区
		float vertices[] = {
			100.0f, 100.0f, 0.0f, 0.0f, 0.0f,
			200.0f, 100.0f, 0.0f, 1.0f, 0.0f,
			200.0f, 200.0f, 0.0f, 1.0f, 1.0f,
			100.0f, 200.0f, 0.0f, 0.0f, 1.0f,
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
			0, 2, 3
		};
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 2 * 3);

		// shader
		m_Shader = std::make_unique<Shader>("res/Shaders/base.shader");
		m_Shader->Bind();

		m_Texture = std::make_unique<Texture>("res/Textures/letter_p.png");
		m_Texture->Bind();
		// 创建正交投影矩阵
		float width = static_cast<float>(Window::GetWindow().GetWidth());
		float height = static_cast<float>(Window::GetWindow().GetHeight());
		m_Proj = glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);

		m_Shader->SetUniform1i("u_Texture", 0);
	}
	TestTexture2D::~TestTexture2D()
	{
	}
	void TestTexture2D::OnUpdate(float delta_time)
	{
	}
	void TestTexture2D::OnRender()
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position1[0], m_Position1[1], 0.0f));
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		Renderer::GetRenderer().Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position2[0], m_Position2[1], 0.0f));
		mvp = m_Proj * m_View * model;
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		Renderer::GetRenderer().Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
	}
	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat2("position1(x,y)", m_Position1, 0.0f, 100.0f);
		ImGui::SliderFloat2("position2(x,y)", m_Position2, 0.0f, 100.0f);
	}
}