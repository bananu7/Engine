#pragma once
#include "Resource.h"
#include "Exception.h"
#include "Vector2.h"
#include <Misc.h>

class CImage :
	public CResource
{
	uint32			m_TexId;

public:
	std::string Load(SLoadParams const& loadParams);
	bool LoadFromFile (const std::string& path);

	/// Binds to currently active texture unit and doesn't change it.
	void Bind ();
	/// Binds to given texture unit, active texture unit is changed to the one given
	void Bind (uint8 textureUnitNum);

	void Unload (void) { }

	int GetWidth();

	inline uint32 GetTexture () const { return m_TexId; }

	CImage();
	~CImage();
};

