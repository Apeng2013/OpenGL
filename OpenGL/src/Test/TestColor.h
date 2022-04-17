#pragma once
#include "Test.h"

namespace Test {

	class TestColor : public Test
	{
	public:
		TestColor();
		~TestColor();

		void OnUpdate(float delta_time);
		void OnRender();
		void OnImGuiRender();
	private:
		float m_Color[4];
	};

}