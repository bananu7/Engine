#pragma once
#include "entity.h"
class CShip :
    public CEntity
{
    float m_MaxAcceleration;

public:
    void Acc();
    void Dec();
    void Turn (bool left);
    void Update ();

    CShip(void);
    ~CShip(void);
};

