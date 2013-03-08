
#include "World.h"
#include "Misc.h"
#include <boost/lexical_cast.hpp>
#include "ResManager.h"
#include "Game.h"
#include "GameObject.h"

namespace engine {

using namespace std;
//-------------------------------------------------------------
void World::_LoadMap()
{
}
//-------------------------------------------------------------
void World::Load ()
{
	/*_LoadMap();
	CVector2<float> Temp (500.0f, 500.0f);
	m_Map = new CMap ();
	m_Map->Load();*/
}
//-------------------------------------------------------------
void World::Draw ()
{
/*	if (m_LocalPlayer)
		m_Map->Draw (m_LocalPlayer->GetPosition());
	else
		m_Map->Draw (CVector2<float>::GetZero());

	for (auto It = m_NetworkPlayers.begin(); It != m_NetworkPlayers.end(); It++)
	{
		(*It).second->Draw();
	}*/

	for (auto It = m_Objects.begin(); It != m_Objects.end(); It++)
	{
		It->second->Draw();
	}
}

//-------------------------------------------------------------

/*CPlayer* World::GetLocalPlayer(void) const
{
	return m_LocalPlayer;
}
//-------------------------------------------------------------
void World::CreateLocalPlayer (uint8 id)
{
	m_LocalPlayer = new CPlayer(id, this);
	m_NetworkPlayers.Add(id, m_LocalPlayer);
}*/
//-------------------------------------------------------------
GameObject* World::GetObjectById (unsigned id)
{
	auto It = m_Objects.find(id);
	if (It != m_Objects.end())
		return It->second;
	else
		return 0;
}
void World::AddObject (GameObject* obj)
{
	auto It = m_Objects.find(obj->GetId());
	if (It != m_Objects.end())
		_CrtDbgBreak();
	else
		m_Objects[obj->GetId()] = obj;
}
//-------------------------------------------------------------
World::World()
{
	//m_LocalPlayer = 0;
}

World::~World(void)
{
	for (auto It = m_Objects.begin(); It != m_Objects.end(); It++)
	{
		delete It->second;
	}
	m_Objects.clear();
}

const float World::UPDATE_INTERVAL = 0.016f;

} // namespace engine