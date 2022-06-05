#pragma once

#include "Test.h"
#include "Geometry/Cube.h"
#include "Geometry/Rectangle.h"
#include "Shader.h"
#include "Texture.h"

namespace Test{
	class TestBlend : public Test
	{
	public:
		TestBlend();
		~TestBlend();

		virtual void OnUpdate(float delta_time) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	private:
		Cube m_Cube;
		Rectangle m_Window_1, m_Window_2;
		Shader m_Shader;
		Texture m_CubeTex, m_WindowTex;
	};
}