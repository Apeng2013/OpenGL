#pragma once

#include "Test.h"

#include "Geometry/Cube.h"
#include "Geometry/Rectangle.h"
#include "Texture.h"


namespace Test {
	class TestSSAO : public Test
	{
	public:
		TestSSAO();
		~TestSSAO();

		void OnUpdate(float delta_time) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		Cube m_Cube;
		Shader m_CubeShader;
		Texture m_CubeTexture;

		Rectangle m_Screen;
		Shader m_ScreenShader;

		Shader m_SSAOShader;

		unsigned int m_GBuffer;
		unsigned int m_PosBuffer;
		unsigned int m_ColorBuffer;
		unsigned int m_NormalBuffer;
		unsigned int m_RenderBuffer;

		unsigned int m_SSAOBuffer;
		unsigned int m_SSAOColorBuffer;
		unsigned int m_SSAORenderBuffer;
		unsigned int m_NoiseTex;

		std::vector<glm::vec3> m_ssaoKernel;
	};
}