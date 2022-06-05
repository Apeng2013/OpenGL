#pragma once
#include "Shader.h"

class Rectangle
{
public:
	Rectangle(float width = 1.0f, float height = 1.0f);
	~Rectangle();

	void Draw(Shader& shader);
private:
	float m_Width, m_Height, m_Length;
	unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
	unsigned int m_Texture;
};