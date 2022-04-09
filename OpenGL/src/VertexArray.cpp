#include <iostream>
#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCALL(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCALL(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl)
{
	Bind();
	vb.Bind();
	const std::vector<LayoutElement>& elements = vbl.GetElements();
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		auto& element = elements[i];
		GLCALL(glEnableVertexAttribArray(i));
		GLCALL(glVertexAttribPointer(
			i, 
			element.count,
			element.type, 
			element.normalized ? GL_TRUE : GL_FALSE, 
			vbl.GetStride(), 
			(void*)element.offset)
		);
	}
}

void VertexArray::Bind() const
{
	GLCALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCALL(glBindVertexArray(0));
}
