#pragma once
#include "Resource.h"
#include "Typedefs.h"

struct SPixel
{
	uint8 R, G, B, A;

	SPixel() { }
	SPixel(uint8 r, uint8 g, uint8 b, uint8 a=255) :
		R(r), G(g), B(b), A(a)
		{ }
};

class CImageData :
	public CResource
{
private:
	int32 m_Height, m_Width;
	int32 m_Channels;
	uint8* m_Data;

public:
	std::string Load (SLoadParams const& params);
	inline SPixel GetPixel (int32 x, int32 y) const
	{
		if (m_Data)
		{
			int Offset = (m_Height * x + y) * m_Channels;
			uint8* Ptr = m_Data + Offset;

			switch (m_Channels)
			{
			case 4:
				return SPixel (Ptr[0], Ptr[1], Ptr[2], Ptr[3]);

			case 3:
				return SPixel (Ptr[0], Ptr[1], Ptr[2]);

			default:
				return SPixel (Ptr[0], Ptr[0], Ptr[0]);
			}
		}
	}
	inline int32 GetHeight () const { return m_Height; }
	inline int32 GetWidth () const { return m_Width; }

	void Unload ();

	CImageData(void);
	~CImageData(void);
};

