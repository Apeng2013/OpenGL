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
	//由上到下按层生成顶点、法线、纹理
	for (int y = 0; y < yCount + 1; y++)
	{
		float yPercent = float(y) / float(yCount);
		for (int x = 0; x < xCount; x++)
		{
			float xPercent = float(x) / float(xCount);

			glm::vec3 pos;
			pos.x = std::sin(PI * yPercent) * std::cos(2.0f * PI * xPercent);
			pos.y = std::cos(PI * yPercent);
			pos.z = std::sin(PI * yPercent) * std::sin(2.0f * PI * xPercent);

			positions.push_back(pos);
			normals.push_back(pos);
			texCoords.push_back(glm::vec2(xPercent, yPercent));
		}
	}

	int vertex_count = positions.size();

	std::vector<unsigned int> indices;

	//处理顶部三角形
	for (int i = 0; i < xCount; i++)
	{
		indices.push_back(i);
		indices.push_back(i + xCount);
		if (i == xCount - 1)
			indices.push_back(xCount);
		else
			indices.push_back(i + xCount + 1);
	}
	//处理底部三角形

	for (int i = 0; i < xCount; i++)
	{
		indices.push_back(vertex_count - 1 - i);
		indices.push_back(vertex_count - 1 - i - xCount);
		if (i == xCount - 1)
			indices.push_back(vertex_count - 1 - xCount);
		else
			indices.push_back(vertex_count - 1 - i - xCount - 1);
	}

	//处理中间的顶点
	//		1-----------4
	//		|			|
	//		|			|
	//		|			|
	//		2-----------3
	// 三角形 （1,2,3）和（1,3,4）
	for (int i = 1; i < yCount; i++)
	{
		int start_index = i * xCount;
		for (int j = 0; j < xCount; j++)
		{
			indices.push_back(start_index + j);
			indices.push_back(start_index + j + xCount);
			if (j == xCount - 1)
				indices.push_back(start_index + xCount);
			else
				indices.push_back(start_index + j + xCount + 1);

			indices.push_back(start_index + j);
			if (j == xCount - 1)
			{
				indices.push_back(start_index + xCount);
				indices.push_back(start_index);
			}
			else
			{
				indices.push_back(start_index + j + 1 + xCount);
				indices.push_back(start_index + j + 1);
			}
		}

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
	glDrawElements(GL_TRIANGLES, m_Count, GL_UNSIGNED_INT, 0);
}
