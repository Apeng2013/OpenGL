#pragma once
#include "Test.h"
#include "Geometry/Sphere.h"

namespace Test {
	class TestPBR : public Test
	{
	public:
		TestPBR();
		~TestPBR();

		void OnUpdate(float delta_time) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Sphere m_Sphere;
		Shader m_Shader;
	};
}