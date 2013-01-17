#include "Image.h"
#include <SOIL.h>
#include <Misc.h>
#include <string>

using std::string;

std::string CImage::Load(ILoader const& loadParams)
{
	auto const& Iter = loadParams.Params.find("path");
	if (Iter == loadParams.Params.end())
		return string("Missing 'path' load param");
	string Path = Iter->second;
	Path = "../Resources/" + Path;

	m_TexId = SOIL_load_OGL_texture
		(
			Path.c_str(),
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

	return (m_TexId != 0) ? std::string() : std::string("General Error");
}

void CImage::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_TexId);
}

void CImage::Bind(uint8 textureUnitNum)
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