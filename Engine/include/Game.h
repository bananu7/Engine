#pragma once
#include "Singleton.h"
#include "LuaVM.h"
#include "GameScripting.hpp"
#include <boost/timer/timer.hpp>
#include <string>
#include <list>
#include <vector>
#include <functional>

class CWindow;
class CWorld;
// Do invoke'a
struct SOption;

/// G³ówna klasa gry.
class CGame :
	public CSingleton<CGame>
{
protected:
	CWorld*					m_World;
	CWindow*				m_Window;
	bool					m_IsServer;
	boost::timer::cpu_timer m_FrameClock;
	bool					m_ShowMap;
	CLuaVM					m_LuaVM;
	
	static std::vector<SUpdateData>	m_UpdateData;

	// Zdarzenia czasowe
	/// Czas rozgrywki
	static boost::timer::cpu_timer		m_GameClock;

	enum EGameState
	{
		GAMESTATE_DOWN = 0,
		GAMESTATE_QUIT,
		GAMESTATE_PLAY,
		GAMESTATE_CONSOLE,
		GAMESTATE_LOAD,
		GAMESTATE_SERVER,
		GAMESTATE_SPLASH,
		GAMESTATE_MENU,
	} m_GameState;

	void _ResolveEvents(void);

public:
	/// Inicjalizuje klasê gry.
	void Init		(const char* lpCmdLine);
	/// Automatyczna metoda dzia³aj¹cego programu.
	void Go			();
	/// Zamkniêcie gry.
	void Destroy	();

	inline CWorld* GetWorld () const { return m_World; }
	float GetLastFrameTime ();
	void ReceivePacket (int16 type, const char* data);

	// dla skryptow
	void GetLuaUpdateData(void); // pobiera dane ze skryptu
	static int LuaGetUpdateData (lua_State* lua); // wysyla dane do skryptu
	static int LuaGetGameVariable (lua_State* lua);

	inline void AddUpdateData (const SUpdateData& data) { m_UpdateData.push_back(data); }

	void Invoke (const std::string& cmd);

	CGame();
	~CGame(void);
};
