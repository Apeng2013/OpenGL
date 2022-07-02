#include "TestHDR.h"
#include <iostream>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"
#include "Camera/Camera.h"

namespace Test {
	TestHDR::TestHDR()
		:m_Cube(), m_CubeTexture("res/Textures/container.png"), m_CubeSpec("res/Textures/container_specular.png"),
		m_CubeShader("res/Shaders/TestHDR/base.shader"), 
		m_Rectangle(2.0f, 2.0f), m_HdrShader("res/Shaders/TestHDR/hdr.shader")
	{
		glGenFramebuffers(1, &m_FrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 960, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

		glGenRenderbuffers(1, &m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 960, 720);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Create Framebuffer failed!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	TestHDR::~TestHDR()
	{
		glDeleteFramebuffers(1, &m_FrameBuffer);
		glDeleteRenderbuffers(1, &m_RenderBuffer);
		glDeleteTextures(1, &m_Texture);
	}

	void TestHDR::OnUpdate(float delta_time)
	{
	}

	void TestHDR::OnRender()
	{
		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();
		Camera& camera = Camera::GetCamera();
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 10.0f));

		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_CubeShader.Bind();
		m_CubeTexture.Bind();
		m_CubeSpec.Bind(1);
		m_CubeShader.SetUniform1i("uTexture", 0);
		m_CubeShader.SetUniform1i("uSpecTex", 1);
		m_CubeShader.SetUniformMat4f("uProj", projection);
		m_CubeShader.SetUniformMat4f("uView", view);
		m_CubeShader.SetUniformMat4f("uModel", model);
		m_CubeShader.SetUniform1i("uShinness", 30);
		m_CubeShader.SetUniform3f("uCameraPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		m_CubeShader.SetUniform3f("uPointLight.position", 0.0f, 0.0f, -4.0f);
		m_CubeShader.SetUniform3f("uPointLight.ambient", 0.1f, 0.1f, 0.1f);
		m_CubeShader.SetUniform3f("uPointLight.diffuse", 1.5f, 1.5f, 1.5f);
		m_CubeShader.SetUniform3f("uPointLight.specular", 20.0f, 20.0f, 20.0f);
		m_CubeShader.SetUniform1f("uPointLight.constant", 1.0f);
		m_CubeShader.SetUniform1f("uPointLight.linear", 0.07f);
		m_CubeShader.SetUniform1f("uPointLight.quadratic", 0.017f);
		m_Cube.Draw(m_CubeShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_HdrShader.Bind();
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		m_HdrShader.SetUniform1i("uTexture", 2);
		m_Rectangle.Draw(m_HdrShader);

	}

	void TestHDR::OnImGuiRender()
	{
	}
}