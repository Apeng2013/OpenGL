#include "TestInstance.h"
#include "glad/glad.h"
#include "Window.h"
#include "Camera/Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

namespace Test {
	TestInstance::TestInstance()
		:m_VertexArray(0), m_VertexBuffer(0), m_IndexBuffer(0),
		m_Shader("res/Shaders/TestInstance/base.shader"), m_Texture("res/Textures/container.png")
	{

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[] = {
			// Back face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			// Front face
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
			// Left face
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
			// Right face
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
			// Bottom face
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
			// Top face
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left    
		};

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, 36 * 5 * sizeof(float), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)12);

		unsigned int indices[] = {
			0, 1, 2,
			3, 4, 5,
			6, 7, 8,
			9, 10, 11,
			12, 13, 14,
			15, 16, 17,
			18, 19, 20,
			21, 22, 23,
			24, 25, 26,
			27, 28, 29,
			30, 31, 32,
			33, 34, 35
		};

		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		glm::mat4 translation[100];
		float interval = M_PI * 2.0f / 100;
		for (int i = 0; i < 100; i++)
		{
			float x = 20.0f * std::cos(interval * i), z = 20.0f * std::sin(interval * i);
			translation[i] = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, z));
		}

		//新建一个顶点缓冲区，用来存放每个实例所对应的模型变换矩阵
		glGenBuffers(1, &m_InstanceBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * 100, &translation[0], GL_STATIC_DRAW);

		//通过设置顶点属性布局，将缓冲区绑定到当前顶点数组
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)16);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)32);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)48);

		//设置属性值刷新的时机：渲染一个实例之后刷新属性值
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
	}
	TestInstance::~TestInstance()
	{
		glDeleteVertexArrays(1, &m_VertexArray);
		glDeleteBuffers(1, &m_VertexBuffer);
		glDeleteBuffers(1, &m_IndexBuffer);
		glDeleteBuffers(1, &m_InstanceBuffer);
	}
	void TestInstance::OnUpdate(float delta_time)
	{
	}
	void TestInstance::OnRender()
	{
		glBindVertexArray(m_VertexArray);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		Camera& camera = Camera::GetCamera();
		glm::mat4 view = camera.GetViewMatrix();

		Window& window = Window::GetWindow();
		glm::mat4 projection = Window::GetWindow().GetProjectionMatrix();

		m_Shader.Bind();
		m_Texture.Bind();
		m_Shader.SetUniform1i("uTexture", 0);
		m_Shader.SetUniformMat4f("uProj", projection);
		m_Shader.SetUniformMat4f("uView", view);
		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, 100);
	}
	void TestInstance::OnImGuiRender()
	{
	}
}