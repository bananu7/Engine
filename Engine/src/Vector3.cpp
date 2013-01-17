
#include "Vector3.h"
#include "Misc.h"

void CVector3::CrossProduct (const CVector3& b)
{
	float Xt = Y * b.Z - Z * b.Y;
	float Yt = Z * b.X - X * b.Z;
	float Zt = X * b.Y - Y * b.X;
	X = Xt;
	Y = Yt;
	Z = Zt;
}

float CVector3::DotProduct (const CVector3& b)
{
	return X*b.X + Y*b.Y + Z*b.Z;
}

float CVector3::Lenght (const CVector3& other) const
{
	return sqrtf (Sqre(other.X - X)+Sqre(other.Y - Y)+Sqre(other.Z - Z));
}

CVector3::CVector3(void)
{
}


CVector3::~CVector3(void)
{
}
