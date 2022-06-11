#pragma once

#include "Test.h"
#include "Geometry/Cube.h"
#include "Geometry/Rectangle.h"
#include "Shader.h"
#include "Texture.h"

namespace Test {
	class TestPostProcess : public Test
	{
	public:
		TestPostProcess();
		~TestPostProcess();

		virtual void OnUpdate(float delta_time) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	private:
		Cube m_Cube;
		Shader m_Shader;
		Texture m_CubeTex;
	};
}