#pragma once
#include <memory>
#include "Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm/glm.hpp"

namespace Test {

	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();
		
		void OnUpdate(float delta_time) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<VertexBufferLayout> m_VertexBufferLayout;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj, m_View, m_Model;

		float m_Position1[2], m_Position2[2];
	};
}