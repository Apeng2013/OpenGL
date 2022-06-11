#pragma once

#include "Test.h"
#include "Geometry/Cube.h"
#include "Geometry/Rectangle.h"
#include "Shader.h"
#include "Texture.h"

namespace Test {
	class TestFrameBuffer : public Test
	{
	public:
		TestFrameBuffer();
		~TestFrameBuffer();

		virtual void OnUpdate(float delta_time) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	private:
		Cube m_Cube;
		Rectangle m_Rectangle;
		Shader m_Shader;
		Texture m_CubeTexture;
		unsigned int m_FrameBuffer;
		unsigned int m_Texture;
		unsigned int m_RenderBuffer;
	};
}