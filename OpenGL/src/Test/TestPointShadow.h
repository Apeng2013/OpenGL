#pragma once

#include "Test.h"
#include "Geometry/Cube.h"
#include "Shader.h"
#include "Texture.h"

namespace Test {
	class TestPointShadow : public Test
	{
	public:
		TestPointShadow();
		~TestPointShadow();

		virtual void OnUpdate(float delta_time) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	private:
		Cube m_Cube;
		Shader m_Shader;
		Shader m_ShadowShader;
		Texture m_CubeTexture;
		unsigned int m_FrameBuffer;
		unsigned int m_Texture;
	};
}