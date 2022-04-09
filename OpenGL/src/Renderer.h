#pragma once

#include <glad/glad.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
public:
	Renderer();
	~Renderer();


	void Clear();
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
};