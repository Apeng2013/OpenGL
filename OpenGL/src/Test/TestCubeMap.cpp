
#include "TestCubeMap.h"
#include <vector>
#include <string>
#include <iostream>
#include "Window.h"
#include "Camera/Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"
#include "stb_image/stb_image.h"


namespace Test {
	TestCubeMap::TestCubeMap()
		:m_Cube(), m_SkyBox(2.0f), m_Shader("res/Shaders/TestCubeMap/base.shader"), m_SkyShader("res/Shaders/TestCubeMap/skybox.shader")
	{
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		LoadCubeMap();
	}

	TestCubeMap::~TestCubeMap()
	{
		glDeleteTextures(1, &m_SkyTexture);
	}
	void TestCubeMap::OnUpdate(float delta_time)
	{

	}

	void TestCubeMap::OnRender()
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyTexture);

		Camera& camera = Camera::GetCamera();
		glm::mat4 view = camera.GetViewMatrix();

		Window& window = Window::GetWindow();
		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

		glm::vec3 camera_pos = camera.GetPosition();

		m_Shader.Bind();
		m_Shader.SetUniform1i("uTexture", 1);
		m_Shader.SetUniformMat4f("uProj", projection);
		m_Shader.SetUniformMat4f("uView", view);
		m_Shader.SetUniformMat4f("uModel", model);
		m_Shader.SetUniform3f("uCameraPos", camera_pos.x, camera_pos.y, camera_pos.z);
		m_Cube.Draw(m_Shader);

		glDepthFunc(GL_LEQUAL);
		view = glm::mat4(glm::mat3(view));
		m_SkyShader.Bind();
		m_SkyShader.SetUniform1i("uTexture", 1);
		m_SkyShader.SetUniformMat4f("uProj", projection);
		m_SkyShader.SetUniformMat4f("uView", view);
		m_SkyBox.Draw(m_SkyShader);
		glDepthFunc(GL_LESS);
	}

	void TestCubeMap::OnImGuiRender()
	{

	}
	void TestCubeMap::LoadCubeMap()
	{
		std::vector<std::string> faces = {
			"res/Textures/skybox/right.jpg",
			"res/Textures/skybox/left.jpg",
			"res/Textures/skybox/top.jpg",
			"res/Textures/skybox/bottom.jpg",
			"res/Textures/skybox/front.jpg",
			"res/Textures/skybox/back.jpg",
		};

		glGenTextures(1, &m_SkyTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyTexture);

		stbi_set_flip_vertically_on_load(0);

		int width, height, channels;
		for (int i = 0; i < faces.size(); i++)
		{
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}