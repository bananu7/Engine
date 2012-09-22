#pragma once
#include <iostream>

template<typename T>
class CVector2
{
public:
	T X, Y;

	void operator() (T x, T y)				{ X = x; Y = y; }
	void operator+= (const CVector2<T>& a)	{ X+=a.X; Y+=a.Y; }
	void operator-= (const CVector2<T>& a)	{ X-=a.X; Y-=a.Y; }
	void operator*= (const CVector2<T>& a)	{ X*=a.X; Y*=a.Y; }
	void operator*= (const T a)				{ X*=a; Y*=a; }
	void operator/= (const CVector2<T>& a)	{ X/=a.X; Y/=a.Y; }
	void operator/= (const T a)				{ X/=a; Y/=a; }
	void operator= (const CVector2<T>& a)	{ X = a.X; Y = a.Y; }

	T DistanceTo (const CVector2<T>& a) const;
	float HeadingTo (const CVector2<T>& a) const;

	static CVector2<T> GetZero() { return CVector2<T>(0, 0); }

	CVector2() : X(0), Y(0) { }
	CVector2(T x, T y) : X(x), Y(y) { }
};

template<typename T>
CVector2<T> operator+ (const CVector2<T>& a, const CVector2<T>& b)
{
	return CVector2<T> (a.X+b.X, a.Y+b.Y);
}
template<typename T>
CVector2<T> operator- (const CVector2<T>& a, const CVector2<T>& b)
{
	return CVector2<T> (a.X-b.X, a.Y-b.Y);
}
template<typename T>
CVector2<T> operator* (const CVector2<T>& a, const T b)
{
	return CVector2<T> (a.X * b, a.Y * b);
}
template<typename T>
CVector2<T> operator/ (const CVector2<T>& a, const T b)
{
	return CVector2<T> (a.X / b, a.Y / b);
}

template<typename T>
bool operator!= (const CVector2<T>& a, const CVector2<T>& b);
template<typename T>
bool operator== (const CVector2<T>& a, const CVector2<T>& b);

template<typename T>
std::istream& operator>> (std::istream& str, CVector2<T>& a) { str >> a.X >> a.Y; return str; }
template<typename T>
std::ostream& operator<< (std::ostream& str, const CVector2<T>& a) { str << a.X << a.Y; return str; }

