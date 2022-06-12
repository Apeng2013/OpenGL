#pragma once

#include "Test.h"
#include "Geometry/Cube.h"
#include "Geometry/Rectangle.h"
#include "Shader.h"
#include "Texture.h"

namespace Test {
	class TestCubeMap : public Test
	{
	public:
		TestCubeMap();
		~TestCubeMap();

		virtual void OnUpdate(float delta_time) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	private:
		Cube m_Cube;
		Cube m_SkyBox;
		Shader m_Shader;
		Shader m_SkyShader;
		unsigned int m_SkyTexture;

	private:
		void LoadCubeMap();
	};
}