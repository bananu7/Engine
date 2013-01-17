
#include "Camera.h"

#define M_PI       3.14159265358979323846

CMatrix4 CCamera::CreateRotation (float x, float y, float z)
{
	CMatrix4 RotX, RotY, RotZ;
	RotX = CMatrix4::CreateIdentity();
	RotX.Data[5] = cosf(x);
	RotX.Data[10] = cosf(x);
	RotX.Data[6] = sinf(x);
	RotX.Data[9] = -sinf(x);

	RotY = CMatrix4::CreateIdentity();
	RotY.Data[0] = cosf(y);
	RotY.Data[8] = sinf(y);
	RotY.Data[2] = -sinf(y);
	RotY.Data[10] = cosf(y);

	RotZ = CMatrix4::CreateIdentity();
	RotZ.Data[0] = cosf(z);
	RotZ.Data[4] = -sinf(z);
	RotZ.Data[1] = sinf(z);
	RotZ.Data[5] = cosf(z);

	RotX.Mult(RotY);
	RotX.Mult(RotZ);
	return RotX;
}

CMatrix4 CCamera::CreateTranslation(float x, float y, float z)
{
	CMatrix4 Temp = CMatrix4::CreateIdentity();
	Temp.Data[12] = x;
	Temp.Data[13] = y;
	Temp.Data[14] = z;
	return Temp;
}

CMatrix4 CCamera::CreateProjection(float fov, float ratio, float nearClip, float farClip)
{
	CMatrix4 Temp = CMatrix4::CreateIdentity();
	float f = 1.0f / tan (fov * (M_PI / 360.0));

    Temp.Data[0] = f / ratio;
    Temp.Data[1 * 4 + 1] = f;
    Temp.Data[2 * 4 + 2] = (farClip + nearClip) / (nearClip - farClip);
    Temp.Data[3 * 4 + 2] = (2.0f * farClip * nearClip) / (nearClip - farClip);
    Temp.Data[2 * 4 + 3] = -1.0f;
    Temp.Data[3 * 4 + 3] = 0.0f;

	return Temp;
}

CMatrix4 CCamera::CreateFrustum(float left, float right, float bottom, float top, float znear, float zfar)
{
	// Based on The Red Book, Appendix G
	CMatrix4 Temp = CMatrix4::CreateIdentity();
	Temp.Data[0] = 2.f * znear / (right - left);
	Temp.Data[5] = 2.f * znear / (top - bottom);
	Temp.Data[10] = - (zfar + znear) / (zfar - znear);
	Temp.Data[15] = 0.f;

	Temp.Data[8] = (right + left) / (right - left);
	Temp.Data[9] = (top + bottom) / (top - bottom);

	Temp.Data[14] = -2.f * zfar * znear / (zfar - znear);
	Temp.Data[11] = -1.f;
	return Temp;
}

CMatrix4 CCamera::CreateOrtho(float left, float top, float right, float bottom, float znear, float zfar)
{
	CMatrix4 Temp = CMatrix4::CreateIdentity();
	Temp.Data[0] = 2.0f / (right - left);
	Temp.Data[5] = 2.0f / (top - bottom);
	Temp.Data[10] = -2.0f / (zfar - znear);
	Temp.Data[12] = -(right+left)/(right-left);
	Temp.Data[13] = -(top+bottom)/(top-bottom);
	Temp.Data[14] = -(zfar+znear)/(zfar-znear);
	return Temp;
}

CMatrix4 CCamera::CreateScale (float x, float y, float z)
{
	CMatrix4 Temp = CMatrix4::CreateIdentity();
	Temp.Data[0] = x;
	Temp.Data[5] = y;
	Temp.Data[10] = z;
	return Temp;
}

// Vector helper functions
// Nie uzylem tutaj CVector3 dla maksymalnej ogolnosci kodu
// res = a cross b;
void crossProduct( float *a, float *b, float *res) {
 
    res[0] = a[1] * b[2]  -  b[1] * a[2];
    res[1] = a[2] * b[0]  -  b[2] * a[0];
    res[2] = a[0] * b[1]  -  b[0] * a[1];
}
void normalize(float *a) 
{ 
    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);
 
    a[0] /= mag;
    a[1] /= mag;
    a[2] /= mag;
}

CMatrix4 CCamera::CreateModelview(CVector3 const& eye, CVector3 const& center)
{
	CMatrix4 Temp;
	float dir[3], right[3], up[3];
 
    up[0] = 0.0f;   up[1] = 1.0f;   up[2] = 0.0f;
 
    dir[0] =  (center.X - eye.X);
    dir[1] =  (center.Y - eye.Y);
    dir[2] =  (center.Z - eye.Z);
    normalize(dir);
 
    crossProduct(dir,up,right);
    normalize(right);
 
    crossProduct(right,dir,up);
    normalize(up);
 
	Temp.Data[0]  = right[0];
    Temp.Data[4]  = right[1];
    Temp.Data[8]  = right[2];
    Temp.Data[12] = 0.0f;
 
    Temp.Data[1]  = up[0];
    Temp.Data[5]  = up[1];
    Temp.Data[9]  = up[2];
    Temp.Data[13] = 0.0f;
 
    Temp.Data[2]  = -dir[0];
    Temp.Data[6]  = -dir[1];
    Temp.Data[10] = -dir[2];
    Temp.Data[14] =  0.0f;
 
    Temp.Data[3]  = 0.0f;
    Temp.Data[7]  = 0.0f;
    Temp.Data[11] = 0.0f;
    Temp.Data[15] = 1.0f;
 
	CMatrix4 Aux = CreateTranslation(-eye.X, -eye.Y, -eye.Z);
	Temp.Mult(Aux);

	return Temp;
}

void CCamera::CalculateProjection()
{
	m_ProjectionMatrix = CreateProjection(45, 16.f/10.f, 0.01f, 2000.f);
}
void CCamera::CalculateView()
{
	m_ViewMatrix = CMatrix4::CreateIdentity();
}

CCamera::CCamera(void)
{
}


CCamera::~CCamera(void)
{
}
