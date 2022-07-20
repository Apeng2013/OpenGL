#pragma once
#include "Test.h"
#include "Geometry/Sphere.h"

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
		Sphere m_Sphere;
		Shader m_Shader;

		float m_Roughness;
		float m_Metallic;
	};
}