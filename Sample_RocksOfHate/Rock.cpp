#include "Rock.h"
#include "Model.h"
#include "ResManager.h"
#include "Misc.h"

CRock::CRock(void)
{
	m_Position.X = (Random()-.5f) * 32.f;
	m_Position.Y = (Random()-.5f) * 20.f;
	m_Velocity.X = (Random()-.5f) * 0.02f;
	m_Velocity.Y = (Random()-.5f) * 0.02f;
	m_RotVelocity.Z = (Random()-.5f) * 0.02f;

	m_CollisionLayer = 2;

	m_Model = CResManager::GetSingleton()->GetResource<CModel>("Ball");
	//m_Model->SetScale(0.005f);
}


CRock::~CRock(void)
{
}
