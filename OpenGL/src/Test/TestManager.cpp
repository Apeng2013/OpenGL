#include "TestManager.h"
#include "imgui.h"

namespace Test {
	TestManager::TestManager()
		:m_CurrentTest(nullptr)
	{
	}

	TestManager::~TestManager()
	{
		if (m_CurrentTest != nullptr)
			delete m_CurrentTest;
		m_CurrentTest = nullptr;
	}

	void TestManager::OnUpdate(float delta_time)
	{
		if (m_CurrentTest != nullptr)
		{
			m_CurrentTest->OnUpdate(delta_time);
		}
	}

	void TestManager::OnRender()
	{
		if (m_CurrentTest != nullptr)
		{
			m_CurrentTest->OnRender();
		}
	}

	void TestManager::OnImGuiRender()
	{
		if (m_CurrentTest != nullptr)
		{
			if (ImGui::Button("->"))
			{
				delete m_CurrentTest;
				m_CurrentTest = nullptr;
				return;
			}
			m_CurrentTest->OnImGuiRender();
			return;
		}
		for (auto t : m_Tests)
		{
			if (ImGui::Button(t.first.c_str()))
			{
				m_CurrentTest = t.second();
				break;
			}
		}
	}
}
