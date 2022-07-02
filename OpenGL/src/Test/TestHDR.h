# pragma once

#include "Test.h"
#include "Geometry/Cube.h"
#include "Geometry/Rectangle.h"
#include "Texture.h"
#include "Shader.h"

namespace Test {
	class TestHDR :public Test
	{
	public:
		TestHDR();
		~TestHDR();

		void OnUpdate(float delta_time) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Cube m_Cube;
		Texture m_CubeTexture;
		Texture m_CubeSpec;
		Shader m_CubeShader;

		Rectangle m_Rectangle;
		Shader m_HdrShader;

		unsigned int m_Texture;
		unsigned int m_RenderBuffer;
		unsigned int m_FrameBuffer;
	};
}