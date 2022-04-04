#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>


struct ShaderSource
{
	std::string vertexShaderSource;
	std::string fragmentShaderSource;
};

//解析shader文件
static ShaderSource PaserShaderFile(const std::string& filepath)
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

	return { shaders[(int)ShaderType::vertex].str(), shaders[(int)ShaderType::fragment].str() };
}

//编译shader
static unsigned int CompileShader(const char* shaderSource, unsigned int shaderType)
{
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char message[255];
		glGetShaderInfoLog(shader, 255, NULL, message);
		std::cout << "Error: Failed To Compile Shader!" << message << std::endl;
		return 0;
	}
	return shader;
}

int main()
{
	GLFWwindow* window;
	if (!glfwInit())
	{
		return -1;
	}
	window = glfwCreateWindow(780, 480, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	//顶点缓冲区
	float vertices[] = {
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	unsigned int vertextBuffer;
	glGenBuffers(1, &vertextBuffer);//1:生成的buffer数量
	glBindBuffer(GL_ARRAY_BUFFER, vertextBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	//顶点布局
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//索引缓冲区
	unsigned int indices[] = {
		0, 1, 2
	};
	unsigned int indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// 解析shader文件
	ShaderSource shaderSource = PaserShaderFile("triangle_shader.shader");
	// 编译shder
	unsigned int vertexShader = CompileShader(shaderSource.vertexShaderSource.c_str(), GL_VERTEX_SHADER);
	unsigned int fragmentShader = CompileShader(shaderSource.fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);
	// 创建program
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		std::cout << "Link Program Failed!" << std::endl;
		return -1;
	}

	glUseProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}