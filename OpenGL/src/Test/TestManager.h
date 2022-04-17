#pragma once
#include "Test.h"
#include <vector>
#include <string>
#include <functional>

namespace Test {
	class TestManager : public Test
	{
	public:
		TestManager();
		~TestManager();

		void OnUpdate(float delta_time) override;
		void OnRender() override;
		void OnImGuiRender() override;

		template<typename T>
		void PushTest(const std::string& testName)
		{
			m_Tests.push_back(std::make_pair(testName, []() {return new T(); }));
		}
	private:
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
		Test* m_CurrentTest;
	};
}
