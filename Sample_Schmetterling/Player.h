#pragma once
#include "entity.h"
#include <Image.h>

class CModelFbx;
class CPhysicsEntity;

class CPlayer :
    public CEntity
{
    float            m_MaxAcceleration;
    CModelFbx*        m_ModelFbx;
    CPhysicsEntity* m_PhysicsObject;
    CImage*            m_Texture;

public:
    void Acc();
    void Dec();
    void Turn (bool left);
    void Update ();
    void Draw();

    CPlayer(void);
    ~CPlayer(void);
};

