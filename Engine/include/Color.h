#pragma once
#include "Typedefs.h"

class CColor
{
public:
	float R, G, B, A;

	CColor() { }
	CColor(float r, float g, float b, float a=1.f) :
		R(r), G(g), B(b), A(a)
		{ }
};