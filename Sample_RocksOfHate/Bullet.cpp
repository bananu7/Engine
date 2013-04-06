#include "Bullet.h"
#include "Model.h"
#include "ResManager.h"


CBullet::CBullet(const CEntity* where)
{
    m_Position = where->GetPosition();
    m_Rotation.Z = where->GetRotation().Z;
    m_Velocity = where->GetVelocity();

    m_Velocity.X -= cosf(m_Rotation.Z) * 1.f;
    m_Velocity.Y -= sinf(m_Rotation.Z) * 1.f;

    m_CollisionLayer = 3;

    m_Model = CResManager::GetSingleton()->GetResource<Model>("Ball");
}


CBullet::~CBullet(void)
{
}
