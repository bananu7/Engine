#pragma once
#include <list>
class CEntity;

class CEntityMgr
{
	std::list<CEntity*> m_Data;
	bool m_Collisions[8][8];

public:
	void Update ();
	void Draw ();

	void AddObject (CEntity* ent) { m_Data.push_back(ent); }

	CEntityMgr(void);
	~CEntityMgr(void);
};

