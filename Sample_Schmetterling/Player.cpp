#include "Player.h"
//#include "Model.h"
#include "ModelFbx.h"
#include "ResManager.h"
#include "Shader.h"
#include "Physics.h"

void CPlayer::Acc()
{
    CVector3 Vel;
    Vel.X = -cosf(m_Rotation.Y) * m_MaxAcceleration;
    Vel.Y = 0.f;
    Vel.Z = -sinf(m_Rotation.Y) * m_MaxAcceleration;
    m_PhysicsObject->SetAddVelocity(Vel);
}

void CPlayer::Dec()
{
    m_Velocity.X *= 0.9f;
    m_Velocity.Z *= 0.9f;
    if (abs(m_Velocity.X) < 0.01f)
        m_Velocity.X = 0.f;
    if (abs(m_Velocity.Z) < 0.01f)
        m_Velocity.Z = 0.f;
    m_PhysicsObject->SetVelocity(m_Velocity);
}

void CPlayer::Turn (bool left)
{
    if (left)
        m_Rotation.Y -= 0.03f;
    else
        m_Rotation.Y += 0.03f;
}

void CPlayer::Update()
{
    //m_Position += m_Velocity;
    //m_Rotation += m_RotVelocity;
    m_Position = m_PhysicsObject->GetPosition();

    static float Time = 0.f;
    Time += 0.016f;
    m_ModelFbx->SetAnimationPose(0, 0, Time);
    /*if (m_Position.X > 24.f)
        m_Position.X = -24.f;
    if (m_Position.X < -24.f)
        m_Position.X = 24.f;
    if (m_Position.Y > 15.f)
        m_Position.Y = -15.f;
    if (m_Position.Y < -15.f)
        m_Position.Y = 15.f;*/
}

void CPlayer::Draw()
{
    m_ModelFbx->Draw(m_Position, -m_Rotation, CVector3(1.f,1.f,1.f));
}

CPlayer::CPlayer(void):
    m_PhysicsObject(CPhysics::GetSingleton()->CreateBall(2.0f, CVector3(150.f, 150.f, 150.f)))
{
    //m_Texture = CResManager::GetSingleton()->GetResource<CImage>("texture_motyl");
    m_ModelFbx = CResManager::GetSingleton()->GetResource<CModelFbx>("cube_fbx");
    m_ModelFbx->m_Shader = CResManager::GetSingleton()->GetResource<Program>("shader_texturing");
    //m_Model->SetScale(0.01f);
    m_Position = CVector3(150.f, 150.f, 150.f);
    m_MaxAcceleration = 10.f;
    m_CollisionLayer = 1;
}

CPlayer::~CPlayer(void)
{
}
