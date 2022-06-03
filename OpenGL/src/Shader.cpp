#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Renderer.h"


Shader::Shader(const std::string& filepath)
{
	ShaderSource shaderSource = PaserShaderFile(filepath);
	m_RendererID = CreateShader(shaderSource.vertexShaderSource, shaderSource.fragmentShaderSource);
}

Shader::~Shader()
{
	GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCALL(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCALL(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	int location = GetUniformLocation(name);
	GLCALL(glUniform1i(location, value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	int location = GetUniformLocation(name);
	GLCALL(glUniform1f(location, value));
}

void Shader::SetUniform3f(const std::string& name, float v1, float v2, float v3)
{
	int location = GetUniformLocation(name);
	GLCALL(glUniform3f(location, v1, v2, v3));
}

void Shader::SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4)
{
	int location = GetUniformLocation(name);
	GLCALL(glUniform4f(location, v1, v2, v3, v4));
}

void Shader::SetUniformMat3f(const std::string& name, glm::mat3& matrix)
{
	int location = GetUniformLocation(name);
	GLCALL(glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
{
	int location = GetUniformLocation(name);
	GLCALL(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}
	GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	//if (location == -1)
	//	std::cout << "Can not find uniform: " << name << std::endl;
	m_UniformLocationCache[name] = location;

	return location;
}

ShaderSource Shader::PaserShaderFile(const std::string& filepath)
{
	enum class ShaderType
	{
		None = -1, vertex = 0, fragment = 1
	};

	std::ifstream fs(filepath);
	std::string line;
	std::stringstream shaders[2];
	ShaderType shaderType;
	while (std::getline(fs, line))
	{
		if (line.find("shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				shaderType = ShaderType::vertex;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				shaderType = ShaderType::fragment;
			}
		}
		else
		{
			shaders[(int)shaderType] << line << '\n';
		}
	}
	return { shaders[0].str(), shaders[1].str() };
}

unsigned int Shader::CompileShader(const char* shaderSource, unsigned int shaderType)
{
	GLCALL(unsigned int shader = glCreateShader(shaderType));
	GLCALL(glShaderSource(shader, 1, &shaderSource, NULL));
	GLCALL(glCompileShader(shader));
	int success;
	GLCALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		char message[255];
		GLCALL(glGetShaderInfoLog(shader, 255, NULL, message));
		std::cout << "Error: Failed To Compile Shader!" << message << std::endl;
		return 0;
	}
	return shader;
}

unsigned int Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
{
	unsigned int vertexShader = CompileShader(vertexSource.c_str(), GL_VERTEX_SHADER);
	unsigned int fragmentShader = CompileShader(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

	// ´´½¨program
	GLCALL(unsigned int program = glCreateProgram());
	GLCALL(glAttachShader(program, vertexShader));
	GLCALL(glAttachShader(program, fragmentShader));
	GLCALL(glLinkProgram(program));

	int success;
	GLCALL(glGetProgramiv(program, GL_LINK_STATUS, &success));
	if (!success)
	{
		char message[255];
		GLCALL(glGetProgramInfoLog(program, 255, NULL, message));
		std::cout << "Error: Failed To Compile Shader!" << message << std::endl;
		return 0;
	}
	GLCALL(glDeleteShader(vertexShader));
	GLCALL(glDeleteShader(fragmentShader));
	return program;
}
