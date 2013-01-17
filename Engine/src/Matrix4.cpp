
#include "Matrix4.h"

void CMatrix4::Mult(CMatrix4 const& other)
{
	float res[16];
 
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			res[j*4 + i] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				res[j*4 + i] += Data[k*4 + i] * other.Data[j*4 + k];
			}
		}
	}
	memcpy(Data, res, 16 * sizeof(float));
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
	memset(Data, 0, sizeof(float)*16);
}

CMatrix4::CMatrix4(void)
{
}


CMatrix4::~CMatrix4(void)
{
}
