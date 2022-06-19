
#include "TestAntiAliasing.h"
#include "Window.h"
#include "Camera/Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"

namespace Test {
	TestAntiAliasing::TestAntiAliasing()
		:m_Cube(), m_Shader("res/Shaders/TestAntiAliasing/base.shader"),
		m_CubeTex("res/Textures/container.png")
	{
		//需要在创建窗口前调用，glfw会创建相应大小的颜色、深度缓冲区
		//glfwWindowHint(GLFW_SAMPLES, samples);
		int samples = 4;

		glEnable(GL_MULTISAMPLE);

		glGenFramebuffers(1, &m_FrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
		//颜色缓冲区，纹理
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_Texture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, 960, 720, GL_TRUE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_Texture, 0);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		//渲染缓冲对象，深度、模板缓冲
		glGenRenderbuffers(1, &m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, 960, 720);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	TestAntiAliasing::~TestAntiAliasing()
	{
		glDisable(GL_MULTISAMPLE);
		glDeleteFramebuffers(1, &m_FrameBuffer);
		glDeleteTextures(1, &m_Texture);
		glDeleteRenderbuffers(1, &m_RenderBuffer);
	}
	void TestAntiAliasing::OnUpdate(float delta_time)
	{

	}

	void TestAntiAliasing::OnRender()
	{
		Camera& camera = Camera::GetCamera();
		glm::mat4 view = camera.GetViewMatrix();

		Window& window = Window::GetWindow();
		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
		glClearColor(0.3, 0.6, 0.8, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		m_Shader.Bind();
		m_CubeTex.Bind();
		m_Shader.SetUniform1i("uTexture", 0);
		m_Shader.SetUniformMat4f("uProj", projection);
		m_Shader.SetUniformMat4f("uView", view);
		m_Shader.SetUniformMat4f("uModel", model);
		m_Cube.Draw(m_Shader);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, 960, 720, 0, 0, 960, 720, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void TestAntiAliasing::OnImGuiRender()
	{

	}
}