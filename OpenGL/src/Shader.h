#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm/glm.hpp>

struct ShaderSource
{
	std::string vertexShaderSource;
	std::string fragmentShaderSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform3f(const std::string& name, float v1, float v2, float v3);
	void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
	void SetUniformMat3f(const std::string& name, glm::mat3& matrix);
	void SetUniformMat4f(const std::string& name, glm::mat4& matrix);
	int GetUniformLocation(const std::string& name);

private:
	ShaderSource PaserShaderFile(const std::string& filepath);
	unsigned int CompileShader(const char* shaderSource, unsigned int shaderType);
	unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);

};