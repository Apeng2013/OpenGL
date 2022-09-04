#pragma once
#include "Test.h"
#include "Geometry/Sphere.h"
#include "Geometry/Cube.h"
#include "Geometry/Rectangle.h"

namespace Test {
	class TestIBL : public Test
	{
	public:
		TestIBL();
		~TestIBL();

		void OnUpdate(float delta_time) override;
		void OnRender() override;
		void OnImGuiRender() override;

		void GenerateAmbientMap();
		void GenerateIrradianceMap();
		void GeneratePreFilterMap();
		void GenerateBRDFMap();

	private:
		//材质球
		Sphere m_Sphere;
		Shader m_Shader;
		float m_Roughness;
		float m_Metallic;

		Cube m_Cube;


		//环境立方体贴图
		unsigned int m_AmbientFrameBuffer;
		unsigned int m_EquirectangularMap;
		unsigned int m_AmbientCubeMap;
		unsigned int m_AmbientRenderBuffer;
		Shader m_EquirectangularToCubeShader;
		Shader m_SkyShader;

		//漫反射积分
		unsigned int m_IrradianceFrameBuffer;
		unsigned int m_IrradianceMap;
		unsigned int m_IrradianceRenderBuffer;
		Shader m_IrradianceShader;

		//预滤波环境贴图
		unsigned int m_PreFilterFrameBuffer;
		unsigned int m_PreFilterMap;
		unsigned int m_PreFilterRenderBuffer;
		Shader m_PreFilterShader;

		unsigned int m_BRDFFrameBuffer;
		unsigned int m_BRDFMap;
		unsigned int m_BRDFRenderBuffer;
		Shader m_BRDFShader;
		Rectangle m_Rectangle;

		bool m_FirstRender;
	};
}