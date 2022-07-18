#pragma once
#include "Test.h"
#include "Geometry/Cube.h"

namespace Test {
	class TestIBL : public Test
	{
	public:
		TestIBL();
		~TestIBL();

		void OnUpdate(float delta_time) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Cube m_Cube;
		Shader m_Shader;
	};
}