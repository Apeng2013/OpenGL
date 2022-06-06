#pragma once

#include "Test.h"
#include "Shader.h"
#include "Texture.h"

namespace Test {
	class TestCullFace : public Test
	{
	public:
		TestCullFace();
		~TestCullFace();

		virtual void OnUpdate(float delta_time) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	private:
		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		Shader m_Shader;
		Texture m_Texture;
	};
}