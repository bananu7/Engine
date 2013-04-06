#pragma once
#include "Vector3.h"

class Model;

class CEntity abstract
{
protected:
    CVector3    m_Position, m_Rotation;
    CVector3    m_Velocity, m_RotVelocity;
    Model*        m_Model;
    char        m_CollisionLayer;

public:
    inline const CVector3& GetPosition () const { return m_Position; }
    inline const CVector3& GetRotation () const { return m_Rotation; }
    inline const CVector3& GetVelocity () const { return m_Velocity; }
    inline const char GetCollisionLayer () const { return m_CollisionLayer; }

    virtual void Update ()
    {
        m_Position += m_Velocity;
        m_Rotation += m_RotVelocity;
    }

    void Draw();

    CEntity(void);
    virtual ~CEntity(void);
};

