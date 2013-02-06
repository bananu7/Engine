#include "Image.h"
#include <GL/glew.h>
#include <SOIL.h>
#include "Misc.h"
#include <vector>
#include <iostream>
#include <iterator>

using std::string;

string CImage::Load(ILoader & loader)
{
	auto v = loader.GetRawData("main");
	if (v) {

		m_TexId = SOIL_load_OGL_texture_from_memory
			(
				v.get().data(),
				v.get().size(),
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS
			);

		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}

	return (m_TexId != 0) ? std::string() : std::string("General Error");
}

void CImage::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_TexId);
}

void CImage::Bind(int textureUnitNum)
{
	glActiveTexture (GL_TEXTURE0 + textureUnitNum);
	glBindTexture(GL_TEXTURE_2D, m_TexId);
}

int CImage::GetWidth()
{
	int size;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &size);
	return size;
}

CImage::CImage() :
	m_TexId(0)
{
}

CImage::~CImage()
{
	if (m_TexId != 0)
		glDeleteTextures( 1, &m_TexId );
}