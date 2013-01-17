
#include "Transform.h"

void CTransform::operator+= (const CTransform& other)
{
	Translation += other.Translation;
	Rotation += other.Rotation;
}

void CTransform::ApplyToOpenGL() const
{
	glTranslatef(Translation.X, Translation.Y, Translation.Z);
	glRotatef(Rotation.X*180.f/3.14f, 1.f, 0.f, 0.f);
	glRotatef(Rotation.Y*180.f/3.14f, 0.f, 1.f, 0.f);
	glRotatef(Rotation.Z*180.f/3.14f, 0.f, 0.f, 1.f);
}

CTransform::CTransform(void):
	Rotation(CVector3::GetZero()),
	Translation(CVector3::GetZero())
{
	
}


CTransform::~CTransform(void)
{
}
