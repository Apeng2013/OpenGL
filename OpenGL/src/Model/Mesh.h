#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "Shader.h"
#include "assimp/scene.h"


struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Tex
{
	unsigned int id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Tex>& textures);
	~Mesh();
	void Draw(Shader& shader);
private:
	unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Tex> m_Textures;
private:
	void InitMesh();
};