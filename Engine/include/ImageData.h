#pragma once
#include "Resource.h"
#include <cstdint>

struct SPixel
{
	uint8_t R, G, B, A;

	SPixel() { }
	SPixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255) :
		R(r), G(g), B(b), A(a)
		{ }
};

class CImageData :
	public CResource
{
private:
	int m_Height, m_Width;
	int m_Channels;
	uint8_t* m_Data;

public:
	std::string Load (ILoader & loader);
	inline SPixel GetPixel (int x, int y) const
	{
		if (m_Data)
		{
			int Offset = (m_Height * x + y) * m_Channels;
			uint8_t* Ptr = m_Data + Offset;

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
	inline int GetHeight () const { return m_Height; }
	inline int GetWidth () const { return m_Width; }

	void Unload ();

	CImageData(void);
	~CImageData(void);
};

