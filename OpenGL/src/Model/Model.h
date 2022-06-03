#pragma once

#include <vector>
#include <string>
#include "Mesh.h"
#include "Shader.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model
{
public:
	Model(const std::string& path);
	~Model();
	void Draw(Shader& shader);

private:
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;

private:
	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Tex> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name);

};