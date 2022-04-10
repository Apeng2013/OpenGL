#include "Texture.h"
#include "stb_image/stb_image.h"
#include "Renderer.h"

Texture::Texture(const std::string filepath)
	:m_FilePath(filepath), m_LocalBuffer(nullptr),
	m_Width(0), m_Height(0), m_BPP(0)
{
	// OpenGL屏幕像素的排列是从屏幕从左下角开始，从左到右，从下到上排列。
	// stb_image默认是从左上角开始，从左到右，从上到下排列。
	// 所以需要设置先翻转图片进行加载
	stbi_set_flip_vertically_on_load(1);
	// m_BPP用来标识通道数，r/g/b/a, 4设置通道数为4
	m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCALL(glGenTextures(1, &m_RendererID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer) 
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GLCALL(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}
