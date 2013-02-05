#include "Matrix4.h"
#include <memory>

using std::array;

void CMatrix4::Mult(CMatrix4 const& other)
{
	array<float, 16> res;
 
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			res[j*4 + i] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				res[j*4 + i] += Data[k*4 + i] * other.Data[j*4 + k];
			}
		}
	}
	Data = res;
}

CMatrix4 CMatrix4::CreateIdentity()
{
	CMatrix4 Temp;
	Temp.SetZero();
	Temp.Data[0] = Temp.Data[5] = Temp.Data[10] = Temp.Data[15] = 1.0f;
	return Temp;
}

void CMatrix4::SetZero()
{
	Data.fill(0.f);
}

CMatrix4::CMatrix4(void)
{
}


CMatrix4::~CMatrix4(void)
{
}
