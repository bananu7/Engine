#include "EntityMgr.h"
#include "Entity.h"
#include <boost/foreach.hpp>

void CEntityMgr::Update()
{
    BOOST_FOREACH (CEntity* i, m_Data)
    {
        i->Update();
    }

    for (auto It = m_Data.begin(); It != m_Data.end(); ++It)
    {
        auto It2 = It;
        It2++;
        for (;It2 != m_Data.end(); ++It2)
        {
            char ColLayerA = (*It)->GetCollisionLayer();
            char ColLayerB = (*It)->GetCollisionLayer();
            
            if (m_Collisions[ColLayerA][ColLayerB])
            {
                const CVector3& PosA = (*It)->GetPosition();
                const CVector3& PosB = (*It2)->GetPosition();

                if (PosA.Lenght(PosB) < 1.f) // Kolizja
                {
                    auto TempIt = It;
                    It++;
                    m_Data.erase(TempIt);
                    m_Data.erase(It2);
                    break;
                }
            }
        }
    }
}

void CEntityMgr::Draw()
{
    BOOST_FOREACH (CEntity* i, m_Data)
    {
        i->Draw();
    }
}

CEntityMgr::CEntityMgr(void)
{
    memset(m_Collisions, 0, sizeof(bool)*8*8);
    m_Collisions[1][2] = m_Collisions[2][1] = true;
    m_Collisions[2][2] = true;
    m_Collisions[2][3] = m_Collisions[3][2] = true;
}


CEntityMgr::~CEntityMgr(void)
{
    BOOST_FOREACH (CEntity* i, m_Data)
    {
        delete i;
        i = 0;
    }
}
