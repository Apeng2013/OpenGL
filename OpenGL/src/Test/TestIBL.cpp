#include "TestIBL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"
#include "Camera/Camera.h"
#include "imgui.h"
#include "stb_image/stb_image.h"
#include "glad/glad.h"
#include <iostream>

namespace Test {
	TestIBL::TestIBL()
		:m_Sphere(), m_Shader("res/Shaders/TestIBL/base.shader"),
		m_Metallic(0.0f), m_Roughness(0.0f), m_AmbientCubeMap(0), m_Cube(),
		m_EquirectangularToCubeShader("res/Shaders/TestIBL/equirectangular_to_cube.shader"),
		m_SkyShader("res/Shaders/TestIBL/sky.shader"),
		m_IrradianceShader("res/Shaders/TestIBL/irradiance.shader"),
		m_FirstRender(true)
	{

		m_Metallic = 0.1f;
		m_Roughness = 0.1f;
		//加载等距柱状投影图
		stbi_set_flip_vertically_on_load(1);
		std::string filepath = "res/Textures/Alexs_Apt_2k.hdr";
		int width, height, channel;
		float* image_ptr = stbi_loadf(filepath.c_str(), &width, &height, &channel, 0);
		glGenTextures(1, &m_EquirectangularMap);
		glBindTexture(GL_TEXTURE_2D, m_EquirectangularMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, image_ptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		stbi_image_free(image_ptr);

		//将等距柱状图投影为立方体贴图
		glGenFramebuffers(1, &m_FrameBuffer1);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer1);

		glGenTextures(1, &m_AmbientCubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_AmbientCubeMap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_LINEAR);
		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
		}

		glGenRenderbuffers(1, &m_AmbientRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_AmbientRenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 512, 512);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_AmbientRenderBuffer);

		//生成漫反射积分
		glGenFramebuffers(1, &m_FrameBuffer2);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer2);

		glGenTextures(1, &m_IrradianceMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_LINEAR);
		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glGenRenderbuffers(1, &m_RenderBuffer2);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer2);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 32, 32);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer2);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	TestIBL::~TestIBL()
	{
		glDeleteTextures(1, &m_EquirectangularMap);
		glDeleteTextures(1, &m_AmbientCubeMap);
		glDeleteFramebuffers(1, &m_FrameBuffer1);
		glDeleteRenderbuffers(1, &m_AmbientRenderBuffer);
		glDeleteFramebuffers(1, &m_FrameBuffer2);
		glDeleteRenderbuffers(1, &m_RenderBuffer2);
		glDeleteTextures(1, &m_IrradianceMap);
	}

	void TestIBL::OnUpdate(float delta_time)
	{
	}

	void TestIBL::OnFirstRender()
	{
		// 等距柱状图转换成立方体贴图 start
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[] =
		{
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_EquirectangularMap);

		glViewport(0, 0, 512, 512);

		m_EquirectangularToCubeShader.Bind();
		m_EquirectangularToCubeShader.SetUniform1i("uEquirectangularMap", 0);
		m_EquirectangularToCubeShader.SetUniformMat4f("uProj", captureProjection);
		for (int i = 0; i < 6; i++)
		{
			m_EquirectangularToCubeShader.SetUniformMat4f("uView", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_AmbientCubeMap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			m_Cube.Draw(m_EquirectangularToCubeShader);
		}
		// 等距柱状图转换成立方体贴图 end
	}

	void TestIBL::OnSecondRender()
	{
		// 预结算漫反射积分 start
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[] =
		{
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
		glViewport(0, 0, 32, 32);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer2);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_AmbientCubeMap);
		m_IrradianceShader.Bind();
		m_IrradianceShader.SetUniform1i("uEnvironmentMap", 0);
		m_IrradianceShader.SetUniformMat4f("uProj", captureProjection);
		for (int i = 0; i < 6; i++)
		{
			m_IrradianceShader.SetUniformMat4f("uView", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			m_Cube.Draw(m_IrradianceShader);
		}
		// 预结算漫反射积分 end
	}

	void TestIBL::OnRender()
	{
		if (m_FirstRender)
		{
			OnFirstRender();
			OnSecondRender();
			m_FirstRender = false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glViewport(0, 0, 960, 720);

		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();
		Camera& camera = Camera::GetCamera();
		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.5f));

		//绘制材质球 start
		m_Shader.Bind();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap);
		m_Shader.SetUniform1i("uIrradianceMap", 1);
		m_Shader.SetUniformMat4f("uProj", projection);
		m_Shader.SetUniformMat4f("uView", view);
		m_Shader.SetUniformMat4f("uModel", model);
		m_Shader.SetUniform3f("uCameraPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		m_Shader.SetUniform3f("uAlbedo", 0.7f, 0.0f, 0.0f);
		m_Shader.SetUniform1f("uMetallic", m_Metallic);
		m_Shader.SetUniform1f("uRoughness", m_Roughness);
		m_Shader.SetUniform1f("uAo", 0.7f);
		m_Sphere.Draw(m_Shader);
		//绘制材质球 end

		//天空盒 begin
		m_SkyShader.Bind();
		m_SkyShader.SetUniformMat4f("uProj", projection);
		m_SkyShader.SetUniformMat4f("uView", view);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_AmbientCubeMap);
		m_SkyShader.SetUniform1i("uTexture", 2);
		m_Cube.Draw(m_SkyShader);
		//天空盒 end

	}

	void TestIBL::OnImGuiRender()
	{
		ImGui::SliderFloat("Metallic", &m_Metallic, 0.0f, 1.0f);
		ImGui::SliderFloat("Roughness", &m_Roughness, 0.0f, 1.0f);
	}
}