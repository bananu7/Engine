
#include "World.h"
#include "Misc.h"
#include <boost/lexical_cast.hpp>
#include "ResManager.h"
#include "Game.h"
#include "GameObject.h"
using namespace std;
//-------------------------------------------------------------
void CWorld::_LoadMap()
{
}
//-------------------------------------------------------------
void CWorld::Load ()
{
	/*_LoadMap();
	CVector2<float> Temp (500.0f, 500.0f);
	m_Map = new CMap ();
	m_Map->Load();*/
}
//-------------------------------------------------------------
void CWorld::Draw ()
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
std::vector<SUpdateData> CWorld::Update ()
{
	std::vector<SUpdateData> UpdateData;

	for (auto It = m_Objects.begin(); It != m_Objects.end(); It++)
	{
		It->second->Update(UPDATE_INTERVAL);
	}

	// TODO :steer
/*	if (m_LocalPlayer)
	{
		m_LocalPlayer->Steer(dt);
	}*/

	//m_ShotManager->Update(dt);
	return UpdateData;
}

/*CPlayer* CWorld::GetLocalPlayer(void) const
{
	return m_LocalPlayer;
}
//-------------------------------------------------------------
void CWorld::CreateLocalPlayer (uint8 id)
{
	m_LocalPlayer = new CPlayer(id, this);
	m_NetworkPlayers.Add(id, m_LocalPlayer);
}*/
//-------------------------------------------------------------
CGameObject* CWorld::GetObjectById (unsigned id)
{
	auto It = m_Objects.find(id);
	if (It != m_Objects.end())
		return It->second;
	else
		return 0;
}
void CWorld::AddObject (CGameObject* obj)
{
	auto It = m_Objects.find(obj->GetId());
	if (It != m_Objects.end())
		_CrtDbgBreak();
	else
		m_Objects[obj->GetId()] = obj;
}
//-------------------------------------------------------------
CWorld::CWorld()
{
	//m_LocalPlayer = 0;
}

CWorld::~CWorld(void)
{
	for (auto It = m_Objects.begin(); It != m_Objects.end(); It++)
	{
		delete It->second;
	}
	m_Objects.clear();
}

const float CWorld::UPDATE_INTERVAL = 0.016f;
