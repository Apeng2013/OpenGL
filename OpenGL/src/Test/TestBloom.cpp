#include "TestBloom.h"
#include <iostream>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"
#include "Camera/Camera.h"

namespace Test {
	TestBloom::TestBloom()
		:m_Cube(), m_CubeTexture("res/Textures/container.png"), m_CubeShader("res/Shaders/TestBloom/base.shader"),
		m_Screen(2.0f, 2.0f), m_ScreenShader("res/Shaders/TestBloom/screen.shader"), m_BloomShader("res/Shaders/TestBloom/bloom.shader"),
		m_LightShader("res/Shaders/TestBloom/light.shader")
	{
		glGenFramebuffers(1, &m_ScreenFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ScreenFrameBuffer);

		glGenTextures(1, &m_ScreenTex);
		glBindTexture(GL_TEXTURE_2D, m_ScreenTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 960, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ScreenTex, 0);

		glGenTextures(1, &m_BrightTex);
		glBindTexture(GL_TEXTURE_2D, m_BrightTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 960, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_BrightTex, 0);

		unsigned int attachments[] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1
		};
		glDrawBuffers(2, attachments);

		glGenRenderbuffers(1, &m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 960, 720);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Create Framebuffer failed!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glGenFramebuffers(1, &m_GaussianFrameBuffer1);
		glBindFramebuffer(GL_FRAMEBUFFER, m_GaussianFrameBuffer1);

		glGenTextures(1, &m_Texture1);
		glBindTexture(GL_TEXTURE_2D, m_Texture1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 960, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture1, 0);

		glGenRenderbuffers(1, &m_RenderBuffer1);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer1);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 960, 720);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer1);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glGenFramebuffers(1, &m_GaussianFrameBuffer2);
		glBindFramebuffer(GL_FRAMEBUFFER, m_GaussianFrameBuffer2);

		glGenTextures(1, &m_Texture2);
		glBindTexture(GL_TEXTURE_2D, m_Texture2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 960, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture2, 0);

		glGenRenderbuffers(1, &m_RenderBuffer2);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer2);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 960, 720);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer2);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	TestBloom::~TestBloom()
	{
		glDeleteFramebuffers(1, &m_ScreenFrameBuffer);
		glDeleteTextures(1, &m_ScreenTex);
		glDeleteTextures(1, &m_BrightTex);
		glDeleteRenderbuffers(1, &m_RenderBuffer);

		glDeleteFramebuffers(1, &m_GaussianFrameBuffer1);
		glDeleteTextures(1, &m_Texture1);
		glDeleteRenderbuffers(1, &m_RenderBuffer1);
		glDeleteFramebuffers(1, &m_GaussianFrameBuffer2);
		glDeleteTextures(1, &m_Texture2);
		glDeleteRenderbuffers(1, &m_RenderBuffer2);
	}

	void TestBloom::OnUpdate(float delta_time)
	{
	}

	void TestBloom::OnRender()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();
		Camera& camera = Camera::GetCamera();
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
		glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, -1.0f));
		glm::mat4 model3 = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
		glm::mat4 model4 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 model5 = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -2.0f));

		glBindFramebuffer(GL_FRAMEBUFFER, m_ScreenFrameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_CubeShader.Bind();
		m_CubeTexture.Bind();
		m_CubeShader.SetUniform1i("uTexture", 0);
		m_CubeShader.SetUniform1i("uSpecTex", 1);
		m_CubeShader.SetUniformMat4f("uProj", projection);
		m_CubeShader.SetUniformMat4f("uView", view);
		m_CubeShader.SetUniform1i("uShinness", 30);
		m_CubeShader.SetUniform3f("uCameraPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		m_CubeShader.SetUniform3f("uPointLight1.position", 0.0f, 0.0f, 0.0f);
		m_CubeShader.SetUniform3f("uPointLight1.ambient", 0.1f, 0.1f, 0.1f);
		m_CubeShader.SetUniform3f("uPointLight1.diffuse", 1.5f, 0.0f, 0.0f);
		m_CubeShader.SetUniform3f("uPointLight1.specular", 20.0f, 0.0f, 0.0f);
		m_CubeShader.SetUniform1f("uPointLight1.constant", 1.0f);
		m_CubeShader.SetUniform1f("uPointLight1.linear", 0.07f);
		m_CubeShader.SetUniform1f("uPointLight1.quadratic", 0.017f);

		m_CubeShader.SetUniform3f("uPointLight2.position", 2.0f, 0.0f, -2.0f);
		m_CubeShader.SetUniform3f("uPointLight2.ambient", 0.1f, 0.1f, 0.1f);
		m_CubeShader.SetUniform3f("uPointLight2.diffuse", 1.5f, 0.0f, 0.0f);
		m_CubeShader.SetUniform3f("uPointLight2.specular", 0.0f, 0.0f, 20.0f);
		m_CubeShader.SetUniform1f("uPointLight2.constant", 1.0f);
		m_CubeShader.SetUniform1f("uPointLight2.linear", 0.07f);
		m_CubeShader.SetUniform1f("uPointLight2.quadratic", 0.017f);

		m_CubeShader.SetUniformMat4f("uModel", model1);
		m_Cube.Draw(m_CubeShader);
		m_CubeShader.SetUniformMat4f("uModel", model2);
		m_Cube.Draw(m_CubeShader);
		m_CubeShader.SetUniformMat4f("uModel", model3);
		m_Cube.Draw(m_CubeShader);

		m_LightShader.Bind();
		m_LightShader.SetUniformMat4f("uProj", projection);
		m_LightShader.SetUniformMat4f("uView", view);
		m_LightShader.SetUniform3f("uColor", 20.0f, 0.0f, 0.0f);
		m_LightShader.SetUniformMat4f("uModel", model4);
		m_Cube.Draw(m_LightShader);
		m_LightShader.SetUniform3f("uColor", 0.0f, 0.0f, 20.0f);
		m_LightShader.SetUniformMat4f("uModel", model5);
		m_Cube.Draw(m_LightShader);

		bool is_first_iter = true;
		bool is_horizontal = true;

		for (int i = 0; i < 10; i++)
		{
			unsigned int buffer = is_horizontal ? m_GaussianFrameBuffer1 : m_GaussianFrameBuffer2;
			glBindFramebuffer(GL_FRAMEBUFFER, buffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			m_BloomShader.Bind();
			glActiveTexture(GL_TEXTURE0);
			unsigned int texture;
			if (is_first_iter)
			{
				texture = m_BrightTex;
				is_first_iter = false;
			}
			else
			{
				texture = is_horizontal ? m_Texture2 : m_Texture1;
			}
			glBindTexture(GL_TEXTURE_2D, texture);
			m_BloomShader.SetUniform1i("uTexture", 0);
			m_BloomShader.SetUniform1i("uIsHorizontal", is_horizontal);
			m_Screen.Draw(m_BloomShader);

			is_horizontal = !is_horizontal;

		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_ScreenShader.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ScreenTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_Texture2);
		m_ScreenShader.SetUniform1i("uTexture", 0);
		m_ScreenShader.SetUniform1i("uBrightTex", 1);
		m_Screen.Draw(m_ScreenShader);

	}

	void TestBloom::OnImGuiRender()
	{
	}
}