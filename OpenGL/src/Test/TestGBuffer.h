#pragma once

#include "Test.h"

#include "Geometry/Cube.h"
#include "Geometry/Rectangle.h"
#include "Texture.h"


namespace Test {
	class TestGBuffer: public Test
	{
	public:
		TestGBuffer();
		~TestGBuffer();

		void OnUpdate(float delta_time) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		Cube m_Cube;
		Shader m_CubeShader;
		Texture m_CubeTexture;

		Rectangle m_Screen;
		Shader m_ScreenShader;

		unsigned int m_FrameBuffer;
		unsigned int m_PosBuffer;
		unsigned int m_ColorBuffer;
		unsigned int m_NormalBuffer;
		unsigned int m_RenderBuffer;
	};
}