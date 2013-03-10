#include "Image.h"
#include <GL/glew.h>
#include "Misc.h"
#include <vector>
#include <iostream>
#include <iterator>
#include <exception>
#include <stdexcept>

#include "FreeImage.h"
#include <cstdio>

namespace engine {

using std::string;
using std::runtime_error;

Image Image::_internalLoad(std::vector<unsigned char>&& vd)
{
	Image temporary;

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

	auto fm = FreeImage_OpenMemory(vd.data(), vd.size());

	//check the file signature and deduce its format
	fif = FreeImage_GetFileTypeFromMemory(fm, vd.size());
	//fif = FreeImage_GetFileType(path);

	//if still unknown, try to guess the file format from the file extension
	//if(fif == FIF_UNKNOWN) 
	//	fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		throw runtime_error("Failed to deduce filetype");

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_LoadFromMemory(fif, fm);
	//	dib = FreeImage_Load(fif, path);
	//if the image failed to load, return failure
	if(!dib)
		throw runtime_error("Image failed to load");

	FreeImage_FlipVertical(dib); 
	dib = FreeImage_ConvertTo32Bits(dib);

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		throw runtime_error("Image checking failed");

	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &temporary.m_TexId);
	//bind to the new texture ID
	temporary.Bind();
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
		border, image_format, GL_UNSIGNED_BYTE, bits);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);
	//Free FreeImage's memory stream object
	FreeImage_CloseMemory(fm);
	
	//return success
	return std::move(temporary);
}

void Image::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_TexId);
}

void Image::Bind(int textureUnitNum)
{
	glActiveTexture (GL_TEXTURE0 + textureUnitNum);
	glBindTexture(GL_TEXTURE_2D, m_TexId);
}

int Image::GetWidth()
{
	int size;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &size);
	return size;
}

Image::Image() :
	m_TexId(0)
{
}

Image::Image(Image&& other) {
	m_TexId = other.m_TexId;
	other.m_TexId = 0;
}

Image& Image::operator= (Image&& other) {
	m_TexId = other.m_TexId;
	other.m_TexId = 0;
	return *this;
}


Image::~Image()
{
	if (m_TexId != 0)
		glDeleteTextures( 1, &m_TexId );
}

} // namespace engine
