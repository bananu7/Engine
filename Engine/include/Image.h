#pragma once
#include "Resource.h"
#include <exception>
#include <string>
#include <Misc.h>

namespace engine {

class CImage :
	public CResource
{
	unsigned m_TexId;

public:
	std::string Load(ILoader const& loader);
	
	/// Binds to currently active texture unit and doesn't change it.
	void Bind ();
	/// Binds to given texture unit, active texture unit is changed to the one given
	void Bind (int textureUnitNum);

	void Unload (void) { }

	int GetWidth();

	inline unsigned GetTexture () const { return m_TexId; }

	CImage();
	~CImage();
};

} // namespace engine