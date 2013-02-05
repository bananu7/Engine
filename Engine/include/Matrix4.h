#pragma once
#include <array>

class CMatrix4
{
public:
	// |0  4  8  12|
	// |1  5  9  13|
	// |2  6  10 14|
	// |3  7  11 15|
	std::array<float, 16> Data;	

	void Mult (CMatrix4 const& other);
	void SetZero ();
	static CMatrix4 CreateIdentity();

	float& operator() (int j, int k) { return Data[k*4+j]; }
	float const& operator() (int j, int k) const { return Data[k*4+j]; }
	inline std::array<float, 16> const& GetRawData() const { return Data; }

	CMatrix4(void);
	~CMatrix4(void);
};

