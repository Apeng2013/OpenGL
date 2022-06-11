#include "TestFrameBuffer.h"
#include "glad/glad.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"
#include "Camera/Camera.h"

namespace Test {
	TestFrameBuffer::TestFrameBuffer()
		:m_Cube(), m_Rectangle(), m_Shader("res/Shaders/TestFrameBuffer/base.shader"), m_CubeTexture("res/Textures/container.png")
	{
		glGenFramebuffers(1, &m_FrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 960, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

		glGenRenderbuffers(1, &m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 960, 720);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Create Framebuffer failed" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	TestFrameBuffer::~TestFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_FrameBuffer);
		glDeleteRenderbuffers(1, &m_RenderBuffer);
		glDeleteTextures(1, &m_Texture);
	}
	void TestFrameBuffer::OnUpdate(float delta_time)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
		glClearColor(0.2, 0.6, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();
		glm::mat4 view = Camera::GetCamera().GetViewMatrix();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, -5.0f));
		m_Shader.Bind();
		m_CubeTexture.Bind();
		m_Shader.SetUniform1i("uTexture", 0);
		m_Shader.SetUniformMat4f("uProj", projection);
		m_Shader.SetUniformMat4f("uView", view);
		m_Shader.SetUniformMat4f("uModel", model);
		m_Cube.Draw(m_Shader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.3, 0.6, 0.8, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_Shader.Bind();
		m_CubeTexture.Bind();
		m_Shader.SetUniform1i("uTexture", 0);
		m_Shader.SetUniformMat4f("uProj", projection);
		m_Shader.SetUniformMat4f("uView", view);
		m_Shader.SetUniformMat4f("uModel", model);
		m_Cube.Draw(m_Shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -5.0f));
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		m_Shader.SetUniform1i("uTexture", 1);
		m_Shader.SetUniformMat4f("uModel", model);
		m_Rectangle.Draw(m_Shader);

	}
	void TestFrameBuffer::OnRender()
	{
	}
	void TestFrameBuffer::OnImGuiRender()
	{
	}
}