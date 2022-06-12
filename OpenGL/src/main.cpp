#include "Renderer.h"
#include "Window.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera/Camera.h"
#include "ImGui/ImGuiLayer.h"
#include "Test/Test.h"
#include "Test/TestManager.h"
#include "Test/TestColor.h"
#include "Test/TestTexture2D.h"
#include "Test/TestBlend.h"
#include "Test/TestCullFace.h"
#include "Test/TestFrameBuffer.h"
#include "Test/TestPostProcess.h"
#include "Test/TestCubeMap.h"
#include "Test/TestGLSL.h"
#include "Test/TestGeometryShader.h"


void OnCursorPosChange(GLFWwindow* window, double xpos, double ypos)
{
	Camera& camera = Camera::GetCamera();
    camera.OnCursorPosChange(xpos, ypos);
}

void OnKeyStateChange(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F1)
	{
		if (action == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
	else if (key == GLFW_KEY_F2)
	{
		if (action == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	Camera& camera = Camera::GetCamera();
    camera.OnKeyStateChange(key, scancode, action, mods);
}


int main()
{

	int width = 960, height = 720;

	Camera& camera = Camera::GetCamera();
	camera.InitParam(width, height);

	Window& window = Window::GetWindow();
	window.Init(width, height, "OpenGL Window");

    Renderer& renderer = Renderer::GetRenderer();

    glfwSetCursorPosCallback(window.GetGLWindow(), OnCursorPosChange);
    glfwSetKeyCallback(window.GetGLWindow(), OnKeyStateChange);

    glfwSetInputMode(window.GetGLWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Test::TestManager test_mgr;
	test_mgr.PushTest<Test::TestColor>("Test Color");
	test_mgr.PushTest<Test::TestTexture2D>("Test Texture2D");
	test_mgr.PushTest<Test::TestBlend>("Test Blend");
	test_mgr.PushTest<Test::TestCullFace>("Test CullFace");
	test_mgr.PushTest<Test::TestFrameBuffer>("Test FrameBuffer");
	test_mgr.PushTest<Test::TestPostProcess>("Test PostProcess");
	test_mgr.PushTest<Test::TestCubeMap>("Test CubeMap");
	test_mgr.PushTest<Test::TestGLSL>("Test GLSL");
	test_mgr.PushTest<Test::TestGeometryShader>("Test GeometryShader");

	ImGuiLayer imgui_layer;
	imgui_layer.OnAttach();

	while (!window.ShouldCloseWindow())
	{
		renderer.Clear();

		test_mgr.OnUpdate(0.0f);
		test_mgr.OnRender();

		imgui_layer.Begin();
		test_mgr.OnImGuiRender();
		imgui_layer.End();

		window.Update();
	}

	window.Close();

	return 0;
}