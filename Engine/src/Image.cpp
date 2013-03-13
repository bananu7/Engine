#include "Image.h"
#include <GL/glew.h>
#include "Misc.h"
#include <vector>
#include <memory>
#include <iostream>
#include <iterator>
#include <exception>
#include <stdexcept>

#include "FreeImage.h"
#include <cstdio>

namespace engine {

using std::string;
using std::runtime_error;

struct FreeImageMemoryDeleter {
	void operator()(FIMEMORY* mem) {
		FreeImage_CloseMemory(mem);
	}
};

struct FreeImageBitmapDeleter {
	void operator()(FIBITMAP* dib) {
		FreeImage_Unload(dib);
	}
};

template<typename T, typename Deleter>
inline std::unique_ptr<T, Deleter> make_unique_raw(T* t, Deleter&& d) {
	return std::unique_ptr<T, Deleter>(t, std::forward<Deleter>(d));
}

Image Image::_internalLoad(std::vector<unsigned char>&& vd)
{
	Image temporary;
	
	// FIXME
	GLenum internal_format = GL_RGBA;
	GLuint image_format = GL_BGRA;
	GLuint level = 0;
	GLuint border = 0;
	
	auto fm = make_unique_raw(FreeImage_OpenMemory(vd.data(), vd.size()), FreeImageMemoryDeleter());

	//check the file signature and deduce its format
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(fm.get(), vd.size());

	//if still unknown, try to guess the file format from the file extension
	//if(fif == FIF_UNKNOWN) 
	//	fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		throw runtime_error("Failed to deduce image filetype");

	//check that the plugin has reading capabilities and load the file
	if(!FreeImage_FIFSupportsReading(fif))
		throw runtime_error("This image type is not supported");

	//auto dib = std::unique_ptr<FIBITMAP, FreeImageBitmapDeleter>(
	//	FreeImage_LoadFromMemory(fif, fm.get()), FreeImageBitmapDeleter());

	auto dib = make_unique_raw(FreeImage_LoadFromMemory(fif, fm.get()), FreeImageBitmapDeleter());

	//	dib = FreeImage_Load(fif, path);

	//if the image failed to load, return failure
	if(!dib)
		throw runtime_error("Image failed to load");

	FreeImage_FlipVertical(dib.get()); 
	dib.reset(FreeImage_ConvertTo32Bits(dib.get()));

	//raw pointer to the image data
	BYTE* bits = FreeImage_GetBits(dib.get());
	//get the image width and height
	unsigned width = FreeImage_GetWidth(dib.get());
	unsigned height = FreeImage_GetHeight(dib.get());
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
