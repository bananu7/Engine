#pragma once
#include "Resource.h"
#include <string>
#include <Misc.h>
#include <boost/range.hpp>
#include "gl_id.h"

namespace engine {

class Image
{
    gl_id m_TexId;

    static Image _internalLoad(std::vector<unsigned char>&& data, bool srgb);

public:
    template<class Range>
    static Image Load (Range&& range, bool loadAsSRGB = false) {
        return std::move(_internalLoad(std::vector<unsigned char>(boost::begin(range), boost::end(range)), loadAsSRGB));
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
