#include "Renderer.h"
#include "Window.h"
#include "ImGui/ImGuiLayer.h"
#include "imgui.h"
#include "Test/TestColor.h"
#include "Test/TestManager.h"
#include "Test/TestTexture2D.h"


int main()
{
	Window& window = Window::GetWindow();
	window.Init(960, 720, "OpenGL Window");

	Renderer& renderer = Renderer::GetRenderer();

	ImGuiLayer imguiLayer;
	imguiLayer.OnAttach();

	Test::TestManager testManager;
	testManager.PushTest<Test::TestColor>("Test Color");
	testManager.PushTest<Test::TestTexture2D>("Test Texture2D");

	while (!window.ShouldCloseWindow())
	{
		renderer.Clear();

		testManager.OnUpdate(0.0f);
		testManager.OnRender();

		imguiLayer.Begin();
		ImGui::Begin("ImGui");
		testManager.OnImGuiRender();
		ImGui::End();
		imguiLayer.End();

		window.Update();
	}

	imguiLayer.OnDetach();
	window.Close();

	return 0;
}