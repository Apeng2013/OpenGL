
#include "TestGLSL.h"
#include "Window.h"
#include "Camera/Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"

namespace Test {
	TestGLSL::TestGLSL()
		:m_Cube(), m_Shader("res/Shaders/TestGLSL/base.shader"),
		m_CubeTex("res/Textures/container.png")
	{

		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glGenBuffers(1, &m_UniformBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBuffer);
		glBufferData(GL_UNIFORM_BUFFER, 128, NULL, GL_STATIC_DRAW);

		unsigned int matrices_index = glGetUniformBlockIndex(m_Shader.GetID(), "Matrices");
		glUniformBlockBinding(m_Shader.GetID(), matrices_index, 2);

		glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_UniformBuffer);
	}

	TestGLSL::~TestGLSL()
	{

	}
	void TestGLSL::OnUpdate(float delta_time)
	{

	}

	void TestGLSL::OnRender()
	{
		Camera& camera = Camera::GetCamera();
		glm::mat4 view = camera.GetViewMatrix();

		Window& window = Window::GetWindow();
		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

		glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, &projection[0][0]);
		glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, &view[0][0]);

		m_Shader.Bind();
		m_CubeTex.Bind();
		m_Shader.SetUniform1i("uTexture", 0);
		m_Shader.SetUniformMat4f("uModel", model);
		m_Cube.Draw(m_Shader);
	}

	void TestGLSL::OnImGuiRender()
	{

	}
}