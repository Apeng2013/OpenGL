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

	ImGuiLayer imgui_layer;
	imgui_layer.OnAttach();

	Test::TestManager testManager;
	testManager.PushTest<Test::TestColor>("Test Color");
	testManager.PushTest<Test::TestTexture2D>("Test Texture2D");

	while (!window.ShouldCloseWindow())
	{
		renderer.Clear();

		testManager.OnUpdate(0.0f);
		testManager.OnRender();

		imgui_layer.Begin();
		ImGui::Begin("ImGui");
		testManager.OnImGuiRender();
		ImGui::End();
		imgui_layer.End();

		window.Update();
	}

	imgui_layer.OnDetach();
	window.Close();

	return 0;
}