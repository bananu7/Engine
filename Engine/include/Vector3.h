#pragma once
#include <cmath>
#include <istream>

class CVector3
{
public:
	float X, Y, Z;

	static inline CVector3 GetZero () { return CVector3(0.0f, 0.0f, 0.0f); }

	void CrossProduct (const CVector3& b);
	float DotProduct (const CVector3& b);
	void Normalize () { float L = sqrtf(X*X+Y*Y+Z*Z); X /= L; Y /= L; Z /= L;}
	float Lenght (const CVector3& other) const;
	 
	CVector3 operator+ (const CVector3& a) const { return CVector3(X+a.X, Y+a.Y, Z+a.Z); }
	CVector3 operator- (const CVector3& a) const { return CVector3(X-a.X, Y-a.Y, Z-a.Z); }
	CVector3 operator- (void) const { return CVector3(-X, -Y, -Z); }
	CVector3 operator* (float a) const { return CVector3(X*a, Y*a, Z*a); }

	void operator+= (const CVector3& a) { X += a.X; Y += a.Y; Z += a.Z; }
	void operator= (const CVector3& a) { X = a.X; Y = a.Y; Z = a.Z; }
	void operator*= (float a) { X *= a; Y *= a; Z *= a; }

	friend std::istream& operator>> (std::istream& in, CVector3& a);

	CVector3(void);
	CVector3 (float x, float y, float z) : X(x), Y(y), Z(z) { }
	~CVector3(void);
};

inline std::istream& operator>> (std::istream& in, CVector3& a)
{
	in >> a.X >> a.Y >> a.Z;
	return in;
}