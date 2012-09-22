#include "Vector2.h"
#include <cmath>
#define M_PI 3.14159265358979323846264338327

template<typename T>
T CVector2<T>::DistanceTo (const CVector2<T>& a) const
{
	T TempX = abs(X - a.X);
	T TempY = abs(Y - a.Y);

	return sqrtf((TempX * TempX) + (TempY * TempY));
}

template<typename T>
float CVector2<T>::HeadingTo (const CVector2<T>& a) const
{
	float l;

	if (X == a.X)
	{
	 if (a.Y < Y)
		 return 0.0f;     
	 else
		 return 180.0f;
	}

	if (Y == a.Y)
	{
	 if (a.X < X)
		 return 270.0f;     
	 else
		 return 90.0f;
	}

	l = atan2f ( (a.Y-Y), (a.X-X) );
	l *= (180.0f/M_PI);
	l += 90.0f;
	if (l<0) l += 360.0f;
		   
	return l;
}
//=============================================================================== GLOBALNE
//-----------------------------------------------------------------------------
template<typename T>
bool operator!= (const CVector2<T>& a, const CVector2<T>& b)
{
	if ((a.X != b.X) || (a.Y != b.Y))
		return true;
	return false;
}
//-----------------------------------------------------------------------------
template<typename T>
bool operator== (const CVector2<T>& a, const CVector2<T>& b)
{
	if ((a.X != b.X) || (a.Y != b.Y))
		return false;
	return true;
}
