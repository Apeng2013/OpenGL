#pragma once

#include "Test.h"
#include "Geometry/Cube.h"
#include "Geometry/Rectangle.h"
#include "Shader.h"
#include "Texture.h"

namespace Test {
	class TestGLSL : public Test
	{
	public:
		TestGLSL();
		~TestGLSL();

		virtual void OnUpdate(float delta_time) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	private:
		Cube m_Cube;
		Shader m_Shader;
		Texture m_CubeTex;
		unsigned int m_UniformBuffer;
	};
}