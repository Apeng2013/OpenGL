#include "TestColor.h"
#include "Renderer.h"
#include "imgui.h"

namespace Test{
	TestColor::TestColor()
		:m_Color{0.1f, 0.1f, 0.7f, 1.0f}
	{
	}
	TestColor::~TestColor()
	{
	}
	void TestColor::OnUpdate(float delta_time)
	{
	}
	void TestColor::OnRender()
	{
		GLCALL(glClearColor(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
	}
	void TestColor::OnImGuiRender()
	{
		ImGui::ColorPicker4("Color", m_Color);
	}
}