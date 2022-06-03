#include "Mesh.h"
#include "glad/glad.h"
#include <iostream>


Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Tex>& textures)
	:m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
{
	InitMesh();
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuse_texture_count = 1;
	unsigned int specular_texture_count = 1;
	for (int i = 0; i < m_Textures.size(); i++) 
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = m_Textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuse_texture_count++);
		else if (name == "texture_specular")
			number = std::to_string(specular_texture_count++);
		else
			continue;
		shader.SetUniform1i("u" + name + number, i);
		glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
	}
	glBindVertexArray(m_VertexArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
 
void Mesh::InitMesh()
{
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);
	
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::texCoords));

	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
