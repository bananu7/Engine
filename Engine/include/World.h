#pragma once
#include <string>
#include <list>
#include <map>
#include "GameScripting.hpp"

class CGameObject;

class CWorld
{
private:
	static const float UPDATE_INTERVAL;

protected:
	std::map<unsigned, CGameObject*> m_Objects;

	void _LoadMap();

public:

	void Load();
	std::vector<SUpdateData> Update ();
	
	void Draw();

	CGameObject* GetObjectById (unsigned id);
	void AddObject (CGameObject* obj);

	CWorld();
	virtual ~CWorld();
};
