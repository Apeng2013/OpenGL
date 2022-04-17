#include "Renderer.h"
#include "Window.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "ImGui/ImGuiLayer.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "Test/TestColor.h"


int main()
{
	Window& window = Window::GetWindow();
	window.Init(960, 720, "OpenGL Window");

	//顶点数组
	VertexArray va;

	//顶点缓冲区
	float vertices[] = {
		100.0f, 100.0f, 0.0f, 0.0f, 0.0f,
		200.0f, 100.0f, 0.0f, 1.0f, 0.0f,
		200.0f, 200.0f, 0.0f, 1.0f, 1.0f,
		100.0f, 200.0f, 0.0f, 0.0f, 1.0f,
	};
	VertexBuffer vb(vertices, 4 * 5 * sizeof(float));
	//顶点布局
	VertexBufferLayout vbl(
		{
			{GL_FLOAT, 3, 0, 0},
			{GL_FLOAT, 2, 0, 0}
		}
	);

	va.AddBuffer(vb, vbl);

	//索引缓冲区
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	IndexBuffer ib(indices, 2 * 3);

	// shader
	Shader shader("res/Shaders/base.shader");
	shader.Bind();

	Texture texture("res/Textures/letter_p.png");
	texture.Bind();


	// 创建正交投影矩阵
	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 720.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 model;
	glm::mat4 mvp;

	shader.SetUniform1i("u_Texture", 0);

	Renderer renderer;

	ImGuiLayer imgui_layer;
	imgui_layer.OnAttach();

	float position1[2] = { 0.0f, 0.0f };
	float position2[2] = { 0.0f, 0.0f };

	Test::TestColor testColor;

	while (!window.ShouldCloseWindow())
	{
		renderer.Clear();

		testColor.OnUpdate(0.0f);
		testColor.OnRender();

		model = glm::translate(glm::mat4(1.0f), glm::vec3(position1[0], position1[1], 0.0f));
		mvp = proj * view * model;
		shader.SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(va, ib, shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(position2[0], position2[1], 0.0f));
		mvp = proj * view * model;
		shader.SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(va, ib, shader);

		imgui_layer.Begin();
		ImGui::Begin("Modify Position");   
		ImGui::SliderFloat2("position1(x,y)", position1, 0.0f, 100.0f);
		ImGui::SliderFloat2("position2(x,y)", position2, 0.0f, 100.0f);
		testColor.OnImGuiRender();
		ImGui::End();
		imgui_layer.End();

		window.Update();
	}

	imgui_layer.OnDetach();
	window.Close();

	return 0;
}