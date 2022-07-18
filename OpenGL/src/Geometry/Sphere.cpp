#include "Sphere.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <vector>

Sphere::Sphere(float radius)
	:m_Radius(radius)
{
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	float PI = 3.1415926f;
	int xCount = 64;
	int yCount = 64;

	for (int x = 0; x < xCount; x++)
	{
		for (int y = 0; y < yCount; y++)
		{
			float xPercent = float(x) / float(xCount);
			float yPercent = float(y) / float(yCount);

			glm::vec3 pos;
			pos.x = std::sin(PI * yPercent) * std::cos(2.0f * PI * xPercent);
			pos.y = std::cos(PI * yPercent);
			pos.z = std::sin(PI * yPercent) * std::sin(2.0f * PI * xPercent);

			positions.push_back(pos);
			normals.push_back(pos);
			texCoords.push_back(glm::vec2(xPercent, yPercent));
		}
	}

	std::vector<unsigned int> indices;

	bool oddRow = false;
	for (unsigned int y = 0; y < yCount; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (unsigned int x = 0; x <= xCount; ++x)
			{
				indices.push_back(y * (xCount + 1) + x);
				indices.push_back((y + 1) * (xCount + 1) + x);
			}
		}
		else
		{
			for (int x = xCount; x >= 0; --x)
			{
				indices.push_back((y + 1) * (xCount + 1) + x);
				indices.push_back(y * (xCount + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	m_Count = indices.size();

	std::vector<float> vertex_buffer;

	for (int i = 0; i < positions.size(); i++)
	{
		vertex_buffer.push_back(positions[i].x);
		vertex_buffer.push_back(positions[i].y);
		vertex_buffer.push_back(positions[i].z);

		vertex_buffer.push_back(normals[i].x);
		vertex_buffer.push_back(normals[i].y);
		vertex_buffer.push_back(normals[i].z);

		vertex_buffer.push_back(texCoords[i].x);
		vertex_buffer.push_back(texCoords[i].y);
	}
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer.size() * sizeof(float), &vertex_buffer[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 8, (void*)12);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(float) * 8, (void*)24);

	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

Sphere::~Sphere()
{
	glDeleteVertexArrays(1, &m_VertexArray);
	glDeleteBuffers(1, &m_VertexBuffer);
	glDeleteBuffers(1, &m_IndexBuffer);
}

void Sphere::Draw(Shader& shader)
{
	glBindVertexArray(m_VertexArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glDrawElements(GL_TRIANGLE_STRIP, m_Count, GL_UNSIGNED_INT, 0);
}
