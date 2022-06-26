#pragma once

#include "Test.h"
#include "Geometry/Cube.h"
#include "Geometry/Rectangle.h"
#include "Shader.h"
#include "Texture.h"

namespace Test {
	class TestShadowMapping : public Test
	{
	public:
		TestShadowMapping();
		~TestShadowMapping();

		virtual void OnUpdate(float delta_time) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	private:
		Cube m_Cube;
		Texture m_CubeTexture;
		Rectangle m_Floor;
		Texture m_FloorTexture;
		Shader m_Shader;
		Shader m_ShadowShader;

		unsigned int m_FrameBuffer;
		unsigned int m_Texture;
	};
}