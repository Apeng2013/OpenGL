#include "TestPointShadow.h"
#include "glad/glad.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"
#include "Camera/Camera.h"

namespace Test {
	TestPointShadow::TestPointShadow()
		:m_Cube(), m_Shader("res/Shaders/TestPointShadow/base.shader"), m_ShadowShader("res/Shaders/TestPointShadow/shadow.shader")
		, m_CubeTexture("res/Textures/container.png")
	{
		glGenFramebuffers(1, &m_FrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
		for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_Texture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Create Framebuffer failed!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	TestPointShadow::~TestPointShadow()
	{
		glDeleteFramebuffers(1, &m_FrameBuffer);
		glDeleteTextures(1, &m_Texture);
	}
	void TestPointShadow::OnUpdate(float delta_time)
	{
	}
	void TestPointShadow::OnRender()
	{
		float far_plane = 40.0f;
		glm::mat4 light_projection = glm::perspective(90.0f, float(1024) / float(1024), 0.1f, far_plane);

		glm::vec3 light_pos(0.0f, 0.0f, 0.0f);
		glm::mat4 light_view[6];
		light_view[0] = glm::lookAt(light_pos, light_pos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		light_view[1] = glm::lookAt(light_pos, light_pos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

		light_view[2] = glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		light_view[3] = glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

		light_view[4] = glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		light_view[5] = glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();
		glm::mat4 view = Camera::GetCamera().GetViewMatrix();
		glm::mat4 model1 = glm::scale(glm::mat4(1.0f), glm::vec3(15.0f, 15.0f, 15.0f));
		glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, -5.0f));
		glm::mat4 model3 = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, -5.0f));
		glm::mat4 model4 = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 4.0f, 4.0f));

		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_ShadowShader.Bind();
		m_ShadowShader.SetUniformMat4f("uProj", light_projection);
		m_ShadowShader.SetUniformMat4f("uLightViews[0]", light_view[0]);
		m_ShadowShader.SetUniformMat4f("uLightViews[1]", light_view[1]);
		m_ShadowShader.SetUniformMat4f("uLightViews[2]", light_view[2]);
		m_ShadowShader.SetUniformMat4f("uLightViews[3]", light_view[3]);
		m_ShadowShader.SetUniformMat4f("uLightViews[4]", light_view[4]);
		m_ShadowShader.SetUniformMat4f("uLightViews[5]", light_view[5]);

		m_ShadowShader.SetUniform3f("uLightPos", light_pos.x, light_pos.y, light_pos.z);
		m_ShadowShader.SetUniform1f("uFarPlane", far_plane);

		m_ShadowShader.SetUniformMat4f("uModel", model1);
		m_Cube.Draw(m_ShadowShader);
		m_ShadowShader.SetUniformMat4f("uModel", model2);
		m_Cube.Draw(m_ShadowShader);
		m_ShadowShader.SetUniformMat4f("uModel", model3);
		m_Cube.Draw(m_ShadowShader);
		m_ShadowShader.SetUniformMat4f("uModel", model4);
		m_Cube.Draw(m_ShadowShader);

		glViewport(0, 0, 960, 720);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.3, 0.6, 0.8, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_Shader.Bind();

		m_CubeTexture.Bind();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
		m_Shader.SetUniform1i("uTexture", 0);
		m_Shader.SetUniform1i("uShadowTex", 1);

		m_Shader.SetUniformMat4f("uProj", projection);
		m_Shader.SetUniformMat4f("uView", view);

		m_Shader.SetUniform3f("uLightPos", light_pos.x, light_pos.y, light_pos.z);
		m_Shader.SetUniform1f("uFarPlane", far_plane);

		m_Shader.SetUniformMat4f("uModel", model1);
		m_Cube.Draw(m_Shader);
		m_Shader.SetUniformMat4f("uModel", model2);
		m_Cube.Draw(m_Shader);
		m_Shader.SetUniformMat4f("uModel", model3);
		m_Cube.Draw(m_Shader);
		m_Shader.SetUniformMat4f("uModel", model4);
		m_Cube.Draw(m_Shader);
	}
	void TestPointShadow::OnImGuiRender()
	{
	}
}