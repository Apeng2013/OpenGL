#include "TestGeometryShader.h"
#include "glad/glad.h"

namespace Test {
	TestGeometryShader::TestGeometryShader()
		:m_Shader("res/Shaders/TestGeometryShader/base.shader")
	{
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[] = {
			-0.5f,  0.5f, // 左上
			0.5f,  0.5f, // 右上
			0.5f, -0.5f, // 右下
			-0.5f, -0.5f  // 左下
		};

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

		unsigned int indices[] = {
			0, 1, 2, 3
		};
		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	}
	TestGeometryShader::~TestGeometryShader()
	{
		glDeleteVertexArrays(1, &m_VertexArray);
		glDeleteBuffers(1, &m_VertexBuffer);
		glDeleteBuffers(1, &m_IndexBuffer);
	}
	void TestGeometryShader::OnUpdate(float delta_time)
	{
	}
	void TestGeometryShader::OnRender()
	{
		m_Shader.Bind();
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glDrawArrays(GL_POINTS, 0, 4);
	}
	void TestGeometryShader::OnImGuiRender()
	{
	}
}