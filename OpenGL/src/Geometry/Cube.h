#pragma once

#include <string>
#include "Shader.h"

class Cube
{
public:
	Cube(float width=1.0f, float height=1.0f, float length=1.0f);
	~Cube();

	void Draw(Shader& shader);
private:
	float m_Width, m_Height, m_Length;
	unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
	unsigned int m_Texture;
};