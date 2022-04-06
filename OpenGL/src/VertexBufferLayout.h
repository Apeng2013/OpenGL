#pragma once
#include <vector>
#include <initializer_list>

struct LayoutElement
{
	unsigned int type;
	unsigned int count;
	unsigned int normalized;
	unsigned int offset;
};

class VertexBufferLayout
{
private:
	std::vector<LayoutElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout(std::initializer_list<LayoutElement> elements);
	~VertexBufferLayout();

	const std::vector<LayoutElement>& GetElements() const { return m_Elements; }
	unsigned int GetStride() const { return m_Stride; }
	
};
