#include "TestSSAO.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera/Camera.h"
#include "Window.h"
#include <random>

namespace Test {
	TestSSAO::TestSSAO()
		:m_Cube(), m_CubeShader("res/Shaders/TestSSAO/base.shader"), m_CubeTexture("res/Textures/container.png"),
		m_Screen(2.0f, 2.0f), m_ScreenShader("res/Shaders/TestSSAO/screen.shader"), 
		m_SSAOShader("res/Shaders/TestSSAO/ssao.shader")
	{
		glGenFramebuffers(1, &m_GBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);

		glGenTextures(1, &m_PosBuffer);
		glBindTexture(GL_TEXTURE_2D, m_PosBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 960, 720, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PosBuffer, 0);

		glGenTextures(1, &m_NormalBuffer);
		glBindTexture(GL_TEXTURE_2D, m_NormalBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 960, 720, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_NormalBuffer, 0);

		glGenTextures(1, &m_ColorBuffer);
		glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 960, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_ColorBuffer, 0);

		unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		glGenRenderbuffers(1, &m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 960, 720);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glGenFramebuffers(1, &m_SSAOBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOBuffer);

		glGenTextures(1, &m_SSAOColorBuffer);
		glBindTexture(GL_TEXTURE_2D, m_SSAOColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 960, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_SSAOColorBuffer, 0);

		glGenRenderbuffers(1, &m_SSAORenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_SSAORenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 960, 720);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_SSAORenderBuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
		std::default_random_engine generator;
		for (int i = 0; i < 64; i++)
		{
			glm::vec3 sample(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator)
			);
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			GLfloat scale = GLfloat(i) / 64.0;
			m_ssaoKernel.push_back(sample);
		}

		std::vector<glm::vec3> ssaoNoise;
		for (GLuint i = 0; i < 16; i++)
		{
			glm::vec3 noise(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				0.0f);
			ssaoNoise.push_back(noise);
		}

		glGenTextures(1, &m_NoiseTex);
		glBindTexture(GL_TEXTURE_2D, m_NoiseTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}

	TestSSAO::~TestSSAO()
	{
		glDeleteFramebuffers(1, &m_GBuffer);
		glDeleteTextures(1, &m_PosBuffer);
		glDeleteTextures(1, &m_NormalBuffer);
		glDeleteTextures(1, &m_ColorBuffer);
		glDeleteRenderbuffers(1, &m_RenderBuffer);
	}

	void TestSSAO::OnUpdate(float delta_time)
	{
	}

	void TestSSAO::OnRender()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();
		Camera& camera = Camera::GetCamera();
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

		glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_CubeShader.Bind();
		m_CubeTexture.Bind();
		m_CubeShader.SetUniform1i("uTexture", 0);
		m_CubeShader.SetUniformMat4f("uProj", projection);
		m_CubeShader.SetUniformMat4f("uView", view);
		m_CubeShader.SetUniformMat4f("uModel", model);
		m_Cube.Draw(m_CubeShader);

		glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_SSAOShader.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_PosBuffer);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_NormalBuffer);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_NoiseTex);
		m_SSAOShader.SetUniform1i("uPositionTex", 0);
		m_SSAOShader.SetUniform1i("uNormalTex", 1);
		m_SSAOShader.SetUniform1i("uNoiseTex", 2);
		m_SSAOShader.SetUniformMat4f("uProj", projection);
		for (int i = 0; i < 64; i++)
		{
			m_SSAOShader.SetUniform3f("uSamples[" + std::to_string(i) + "]", m_ssaoKernel[i].x, m_ssaoKernel[i].y, m_ssaoKernel[i].z);
		}
		m_Screen.Draw(m_SSAOShader);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_ScreenShader.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_PosBuffer);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_NormalBuffer);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_SSAOColorBuffer);
		m_ScreenShader.SetUniform1f("uShinness", 10.0);
		m_ScreenShader.SetUniform1i("uPositionTex", 0);
		m_ScreenShader.SetUniform1i("uNormalTex", 1);
		m_ScreenShader.SetUniform1i("uColorTex", 2);
		m_ScreenShader.SetUniform1i("uSSAOTex", 3);
		m_ScreenShader.SetUniform3f("uCameraPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		m_ScreenShader.SetUniform3f("uPointLight.position", 0.0f, 0.0f, 0.0f);
		m_ScreenShader.SetUniform3f("uPointLight.ambient", 0.2f, 0.2f, 0.2f);
		m_ScreenShader.SetUniform3f("uPointLight.diffuse", 0.5f, 0.5f, 0.5f);
		m_ScreenShader.SetUniform3f("uPointLight.specular", 4.0f, 4.0f, 4.0f);
		m_ScreenShader.SetUniform1f("uPointLight.constant", 1.0f);
		m_ScreenShader.SetUniform1f("uPointLight.linear", 0.07f);
		m_ScreenShader.SetUniform1f("uPointLight.quadratic", 0.017f);
		m_Screen.Draw(m_ScreenShader);
	}

	void TestSSAO::OnImGuiRender()
	{
	}
}
