#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "glad/glad.h"

static unsigned int GetSizeByType(unsigned int type)
{
	switch (type) 
	{
		case GL_INT:			return 4;
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
	}
	ASSERT(false);
	return 0;
}

VertexBufferLayout::VertexBufferLayout(std::initializer_list<LayoutElement> elements)
	:m_Elements(elements), m_Stride(0)
{
	unsigned int offset = 0;
	m_Stride = 0;
	for (auto& element : m_Elements)
	{
		element.offset = offset;
		offset += element.count * GetSizeByType(element.type);
	}
	m_Stride = offset;
}

VertexBufferLayout::~VertexBufferLayout()
{
}
