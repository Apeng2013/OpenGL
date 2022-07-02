#pragma once

#include "Test.h"
#include "Geometry/Cube.h"
#include "Geometry/Rectangle.h"
#include "Shader.h"
#include "Texture.h"

namespace Test {
	class TestParallaxMapping : public Test
	{
	public:
		TestParallaxMapping();
		~TestParallaxMapping();

		virtual void OnUpdate(float delta_time) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	private:
		Rectangle m_Wall;
		Shader m_Shader;
		Texture m_WallTex;
		Texture m_NormalTex;
		Texture m_DepthTex;
	};
}