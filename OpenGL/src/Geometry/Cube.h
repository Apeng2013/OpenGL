#pragma once

#include <string>
#include "Shader.h"

class Cube
{
public:
	Cube(float size=1.0f);
	~Cube();

	void Draw(Shader& shader);
private:
	float m_Size;
	unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
};