#include "Image.h"
#include <GL/glew.h>
#include "Misc.h"
#include <vector>
#include <iostream>
#include <iterator>

#include "FreeImage.h"
#include <cstdio>

using std::string;

string CImage::Load(ILoader const& loader)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	
	// FIXME
	GLenum internal_format = GL_RGBA;
	GLuint image_format = GL_BGRA;
	GLuint level = 0;
	GLuint border = 0;
	
	// TEMP
	//const char* path = "../data/Untitled.png";

	auto v = loader.GetRawData("path");
	auto & vd = v.get();

	auto fm = FreeImage_OpenMemory(vd.data(), vd.size());

	//check the file signature and deduce its format
	fif = FreeImage_GetFileTypeFromMemory(fm, vd.size());
	//fif = FreeImage_GetFileType(path);

	//if still unknown, try to guess the file format from the file extension
	//if(fif == FIF_UNKNOWN) 
	//	fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		return "Failed to deduce filetype";

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_LoadFromMemory(fif, fm);
	//	dib = FreeImage_Load(fif, path);
	//if the image failed to load, return failure
	if(!dib)
		return "Image failed to load";

	FreeImage_FlipVertical(dib); 
	dib = FreeImage_ConvertTo32Bits(dib);

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return "Image checking failed";

	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &m_TexId);
	//bind to the new texture ID
	Bind();
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
		border, image_format, GL_UNSIGNED_BYTE, bits);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);
	//Free FreeImage's memory stream object
	FreeImage_CloseMemory(fm);
	
	//return success
	return "";
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