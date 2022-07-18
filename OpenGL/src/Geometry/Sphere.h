#pragma once
#include "Shader.h"

class Sphere
{
public:
	Sphere(float radius=1.0f);
	~Sphere();

	void Draw(Shader& shader);
private:
	float m_Radius;
	unsigned int m_Count;
	unsigned int m_VertexArray;
	unsigned int m_VertexBuffer;
	unsigned int m_IndexBuffer;
};