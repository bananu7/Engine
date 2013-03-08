#pragma once
#include "Resource.h"
#include <string>
#include <Misc.h>
#include <boost/range.hpp>
#include <boost/noncopyable.hpp>

namespace engine {

class Image : public boost::noncopyable
{
	unsigned m_TexId;

	static Image Image::_internalLoad(std::vector<unsigned char>&& data);

public:
	template<class Range>
	static Image Load (Range&& range) {
		return std::move(_internalLoad(std::vector<unsigned char>(boost::begin(range), boost::end(range))));
	}
	
	/// Binds to currently active texture unit and doesn't change it.
	void Bind ();
	/// Binds to given texture unit, active texture unit is changed to the one given
	void Bind (int textureUnitNum);

	void Unload (void) { }

	int GetWidth();

	inline unsigned GetTexture () const { return m_TexId; }

	Image();
	Image(Image&& other);
	Image& operator=(Image&& other);
	~Image();
};

} // namespace engine