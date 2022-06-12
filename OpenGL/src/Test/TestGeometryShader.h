#include <memory>
#include "Test.h"
#include "Shader.h"

namespace Test {

	class TestGeometryShader : public Test
	{
	public:
		TestGeometryShader();
		~TestGeometryShader();

		void OnUpdate(float delta_time) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		unsigned int m_VertexArray;
		unsigned int m_VertexBuffer;
		unsigned int m_IndexBuffer;
		Shader m_Shader;
	};
}