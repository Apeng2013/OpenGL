#pragma once
#include "Test.h"
#include "Geometry/Sphere.h"
#include "Geometry/Cube.h"

namespace Test {
	class TestIBL : public Test
	{
	public:
		TestIBL();
		~TestIBL();

		void OnUpdate(float delta_time) override;
		void OnRender() override;
		void OnImGuiRender() override;

		void OnFirstRender();
		void OnSecondRender();

	private:
		//材质球
		Sphere m_Sphere;
		Shader m_Shader;
		float m_Roughness;
		float m_Metallic;

		Cube m_Cube;


		//环境立方体贴图
		unsigned int m_FrameBuffer1;
		unsigned int m_EquirectangularMap;
		unsigned int m_AmbientCubeMap;
		unsigned int m_AmbientRenderBuffer;
		Shader m_EquirectangularToCubeShader;
		Shader m_SkyShader;

		//漫反射积分
		unsigned int m_FrameBuffer2;
		unsigned int m_IrradianceMap;
		unsigned int m_RenderBuffer2;
		Shader m_IrradianceShader;

		bool m_FirstRender;
	};
}