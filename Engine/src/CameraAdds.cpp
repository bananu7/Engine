#include "StdAfx.h"
#include "CameraAdds.h"
#include "Misc.h"

void CCameraSimple::CalculateView()
{
	m_ViewMatrix = CreateModelview(Position, Target);
}

void CCameraFly::CalculateView()
{
	CMatrix4 RMatrix = CCamera::CreateRotation(LookDir.Y, LookDir.X, 0.f);
	CMatrix4 TMatrix = CCamera::CreateTranslation(-Position.X, -Position.Y, -Position.Z);
	m_ViewMatrix = CMatrix4::CreateIdentity();
	m_ViewMatrix.Mult(RMatrix);
	m_ViewMatrix.Mult(TMatrix);
}
void CCameraFly::Fly (float len)
{
	float Pi = 3.1416;
	//Position.X += cosf(LookDir.X - Pi/2.f) * len;
	//Position.Z += sinf(LookDir.X - Pi/2.f) * len;
	CMatrix4 RMatrix = CCamera::CreateRotation(LookDir.X, LookDir.Y, 0.f);

	CVector3 Norm (0.f, 0.f, 1.f);
	CVector3 Delta;
	Delta.Y = RMatrix.Data[8];
	Delta.X = RMatrix.Data[9];
	Delta.Z = RMatrix.Data[10];

	Delta *= -len;
	Position += Delta;
}

void CCameraFly::Strafe(float left)
{
	Position.X -= cosf(LookDir.X) * left;
	Position.Z -= sinf(LookDir.X) * left;
}

void CCameraTrack::CalculateView()
{
	CVector3 Delta;
	Delta.Y = 5.0f;
	Delta.X = Radius * cosf(Direction/* + M_PI * .5f*/);
	Delta.Z = Radius * sinf(Direction/* + M_PI * .5f*/);
	m_ViewMatrix = CCamera::CreateModelview(Delta + Target, Target);
}