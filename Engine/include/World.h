#pragma once
#include "Typedefs.h"
#include <list>
#include <map>
#include "GameScripting.hpp"

class CGameObject;

class CWorld
{
private:
	static const float UPDATE_INTERVAL;

protected:
	//CPlayer*							m_LocalPlayer;
	std::map<uint32, CGameObject*>		m_Objects;

	void _LoadMap(void);

public:

	void Load (void);
	std::vector<SUpdateData> Update ();
	//TEMP:
	
	void Draw (void);

	//CPlayer* GetLocalPlayer(void) const;
	//void CreateLocalPlayer (uint8 id = 1);

	CGameObject* GetObjectById (uint32 id);
	void AddObject (CGameObject* obj);

	CWorld();
	virtual ~CWorld(void);
};
