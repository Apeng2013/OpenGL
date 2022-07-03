# pragma once

#include "Test.h"
#include "Geometry/Cube.h"
#include "Geometry/Rectangle.h"
#include "Texture.h"
#include "Shader.h"

namespace Test {
	class TestBloom :public Test
	{
	public:
		TestBloom();
		~TestBloom();

		void OnUpdate(float delta_time) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Cube m_Cube;
		Texture m_CubeTexture;
		Shader m_CubeShader;

		Rectangle m_Screen;
		Shader m_ScreenShader;
		Shader m_BloomShader;
		Shader m_LightShader;

		unsigned int m_ScreenFrameBuffer;
		unsigned int m_ScreenTex;
		unsigned int m_BrightTex;
		unsigned int m_RenderBuffer;


		unsigned int m_GaussianFrameBuffer1;
		unsigned int m_Texture1;
		unsigned int m_RenderBuffer1;
		unsigned int m_GaussianFrameBuffer2;
		unsigned int m_Texture2;
		unsigned int m_RenderBuffer2;
	};
}