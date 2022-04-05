#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>


#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (unsigned int error = glGetError())
	{
		std::cout << "[OpenGL Error] " << error << " Function: " << function 
			<< " File: " << file << " Line: " << line << std::endl;
		return false;
	}
	return true;
}


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

//创建shader
unsigned int CreateShader(const char* vertexSource, const char* fragmentSource)
{
	unsigned int vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
	unsigned int fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

	// 创建program
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

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(780, 480, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	//顶点数组
	unsigned int vertexArray;
	GLCALL(glGenVertexArrays(1, &vertexArray));
	GLCALL(glBindVertexArray(vertexArray));

	//顶点缓冲区
	float vertices[] = {
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	unsigned int vertextBuffer;
	GLCALL(glGenBuffers(1, &vertextBuffer));//1:生成的buffer数量
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, vertextBuffer));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), vertices, GL_STATIC_DRAW));
	//顶点布局
	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));

	//索引缓冲区
	unsigned int indices[] = {
		0, 1, 2
	};
	unsigned int indexBuffer;
	GLCALL(glGenBuffers(1, &indexBuffer));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	// 解析shader文件
	ShaderSource shaderSource = PaserShaderFile("triangle_shader.shader");
	// 编译shder
	unsigned int program = CreateShader(shaderSource.vertexShaderSource.c_str(), shaderSource.fragmentShaderSource.c_str());

	GLCALL(glUseProgram(program));
	int location = glGetUniformLocation(program, "u_Color");
	ASSERT(location != -1);
	GLCALL(glUniform4f(location, 0.5f, 0.2f, 0.3f, 1.0f));

	float r = 0.0f;
	float interval = 0.05f;

	while (!glfwWindowShouldClose(window))
	{
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
		GLCALL(glClearColor(0.1f, 0.1f, 0.2f, 1.0f));

		GLCALL(glUniform4f(location, r, 0.2f, 0.3f, 1.0f));
		if (r > 1.0f)
		{
			interval = -0.05f;
		}
		else if (r < 0.0f)
		{
			interval = 0.05f;
		}
		r += interval;

		GLCALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}