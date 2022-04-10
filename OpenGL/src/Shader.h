#pragma once
#include <string>
#include <unordered_map>

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

	void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
	void SetUniform1i(const std::string& name, int value);
	int GetUniformLocation(const std::string& name);

private:
	ShaderSource PaserShaderFile(const std::string& filepath);
	unsigned int CompileShader(const char* shaderSource, unsigned int shaderType);
	unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);

};