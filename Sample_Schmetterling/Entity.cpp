#include "Entity.h"
#include "Model.h"

void CEntity::Draw()
{
    m_Model->Draw(m_Position, m_Rotation, CVector3(1.f,1.f,1.f));
}

CEntity::CEntity(void)
{
    m_Position = CVector3::GetZero();
    m_Rotation = CVector3::GetZero();
    m_Velocity = CVector3::GetZero();
    m_RotVelocity = CVector3::GetZero();
}


CEntity::~CEntity(void)
{
}
