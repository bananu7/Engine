#include "Ship.h"
#include "ModelFbx.h"
#include "ResManager.h"

void CShip::Acc()
{
	m_Velocity.X -= cosf(m_Rotation.Z) * m_MaxAcceleration;
	m_Velocity.Y -= sinf(m_Rotation.Z) * m_MaxAcceleration;
}

void CShip::Dec()
{
	m_Velocity.X *= 0.9f;
	m_Velocity.Y *= 0.9f;
	if (abs(m_Velocity.X) < 0.01f)
		m_Velocity.X = 0.f;
	if (abs(m_Velocity.Y) < 0.01f)
		m_Velocity.Y = 0.f;
}

void CShip::Turn (bool left)
{
	if (left)
		m_Rotation.Z -= 0.3f;
	else
		m_Rotation.Z += 0.3f;
}

void CShip::Update()
{
	m_Position += m_Velocity;
	m_Rotation += m_RotVelocity;

	if (m_Position.X > 24.f)
		m_Position.X = -24.f;
	if (m_Position.X < -24.f)
		m_Position.X = 24.f;
	if (m_Position.Y > 15.f)
		m_Position.Y = -15.f;
	if (m_Position.Y < -15.f)
		m_Position.Y = 15.f;
}

CShip::CShip(void)
{
	m_Model = CResManager::GetSingleton()->GetResource<CModelFbx>("motyl.FBX");
	//m_Model->SetScale(0.01f);
	m_MaxAcceleration = 0.01f;
	m_CollisionLayer = 1;
}


CShip::~CShip(void)
{
}
