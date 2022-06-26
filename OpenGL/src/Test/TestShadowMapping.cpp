#include "TestShadowMapping.h"
#include "glad/glad.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"
#include "Camera/Camera.h"

namespace Test {
	TestShadowMapping::TestShadowMapping()
		:m_Cube(), m_CubeTexture("res/Textures/container.png"), 
		m_Floor(20.0f, 20.0f), m_FloorTexture("res/Textures/container.png"),
		m_Shader("res/Shaders/TestShadowMapping/base.shader"), m_ShadowShader("res/Shaders/TestShadowMapping/shadow.shader")
	{
		glGenFramebuffers(1, &m_FrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Texture, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Create Framebuffer failed" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	TestShadowMapping::~TestShadowMapping()
	{
		glDeleteFramebuffers(1, &m_FrameBuffer);
		glDeleteTextures(1, &m_Texture);
	}

	void TestShadowMapping::OnUpdate(float delta_time)
	{
	}

	void TestShadowMapping::OnRender()
	{

		glm::mat4 light_projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 15.0f);
		glm::mat4 light_view = glm::lookAt(glm::vec3(0.0f, 10.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();
		glm::mat4 view = Camera::GetCamera().GetViewMatrix();
		glm::mat4 cube_model1 = glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(0.0f, 3.0f, -0.5f));
		cube_model1 = glm::translate(cube_model1, glm::vec3(0.0f, 3.0f, -5.5f));
		glm::mat4 cube_model2 = glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(2.0f, 1.0f, -0.5f));
		cube_model2 = glm::translate(cube_model2, glm::vec3(1.0f, 2.5f, -7.5f));
		glm::mat4 floor_model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_ShadowShader.Bind();
		m_ShadowShader.SetUniformMat4f("uProj", light_projection);
		m_ShadowShader.SetUniformMat4f("uView", light_view);
		m_ShadowShader.SetUniformMat4f("uModel", cube_model1);
		m_Cube.Draw(m_ShadowShader);
		m_ShadowShader.SetUniformMat4f("uModel", cube_model2);
		m_Cube.Draw(m_ShadowShader);
		m_ShadowShader.SetUniformMat4f("uModel", floor_model);
		m_Floor.Draw(m_ShadowShader);

		glViewport(0, 0, 960, 720);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.3, 0.6, 0.8, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_Shader.Bind();
		m_CubeTexture.Bind();
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		m_Shader.SetUniform1i("uTexture", 0);
		m_Shader.SetUniform1i("uShadowTex", 1);
		m_Shader.SetUniformMat4f("uLightProj", light_projection);
		m_Shader.SetUniformMat4f("uLightView", light_view);
		m_Shader.SetUniformMat4f("uProj", projection);
		m_Shader.SetUniformMat4f("uView", view);
		m_Shader.SetUniformMat4f("uModel", cube_model1);
		m_Cube.Draw(m_Shader);
		m_Shader.SetUniformMat4f("uModel", cube_model2);
		m_Cube.Draw(m_Shader);
		m_Shader.SetUniformMat4f("uModel", floor_model);
		m_Floor.Draw(m_Shader);
	}
	void TestShadowMapping::OnImGuiRender()
	{
	}
}