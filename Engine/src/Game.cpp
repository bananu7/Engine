#include "stdafx.h"
#include "Game.h"
#include "Exception.h"
#include "Misc.h"
#include "ResManager.h"
#include "Logger.h"
#include "GameObject.h"
#include "World.h"
#include <lua.hpp>
#include <boost/lexical_cast.hpp>
#include <functional>
using namespace std;
#define CGUI_SPACE_CHAR 127

//-------------------------------------------------------
void CGame::Init (const char* lpCmdLine)
{
	srand (172);
	string CmdLine(lpCmdLine);
	string Param = CutSection(CmdLine, ' ');
	Param = ToLower(Param);

	// £adowane na serwie jak i na kliencie
	// TODO: Serwer nie potrzebuje surowcow!
	// Zaladuj katalog surowcow
	CResManager::GetSingleton()->LoadWholeFolder("");
	// Zmien aliasy albo zaladuj spoza katalogu surowcow
	CResManager::GetSingleton()->LoadCatalog("resources.ini");

	// Jeœli gra nie jest serwerem, podnosi grafikê i GUI
	CLogger::GetSingleton()->Enable (ELogOutput::TXT | ELogOutput::DEBUG);
	LOG ("Passive Destruction Client Starting\n");

	//CConfig GameConf ("game.ini");
/*	int Xres = GameConf.m_Options["Graphics"]["Xresolution"].Number.Int;
	int Yres = GameConf.m_Options["Graphics"]["Yresolution"].Number.Int;
	bool FullScreen = (bool)(GameConf.m_Options["Graphics"]["FullScreen"].Number.Int);*/
	//CGraphics::GetSingleton()->Init(SGraphicsInitParams(FullScreen, Xres, Yres));
//	m_Graphics->LoadShaders();

	//m_LuaVM.RegisterFunction (Func, "call");
	//m_LuaVM.RegisterFunction (&CGUI::LuaMessage, "luamessage");
	m_LuaVM.RegisterFunction (&CGame::LuaGetUpdateData, "getUpdateData");
	m_LuaVM.RegisterFunction (&CGame::LuaGetGameVariable, "getGameVariable");
	m_LuaVM.Load ("../Resources/scripts/hello.lua");
	m_LuaVM.Start();

	//CGUI::GetSingleton()->Message ("AZaz09,./'][{}!@#$%^&*()-_=+`~");

	//void (CGame::* TempInvokeFuncPtr)(string) = &CGame::Invoke;

	//ESS:
	//m_GameClock.reset();
	m_GameState = EGameState::GAMESTATE_CONSOLE;
}
//--------------------------------------------------------------------------------------------------------------
void CGame::Invoke (const string& cmd)
{  
	m_LuaVM.Execute (cmd);
}
//======================================================================
void CGame::_ResolveEvents (void) 
{
	/*static bool bSwitchToConsole;
	sf::Event Event;

	while (CGraphics::GetSingleton()->GetWindow().PollEvent(Event))
	{
		switch (m_GameState)
		{
		case GAMESTATE_SERVER:
		case GAMESTATE_PLAY:
			switch (Event.Type)
			{
			case sf::Event::KeyPressed:
				switch (Event.Key.Code)
				{
				case sf::Key::Tilde:
					//if (bSwitchToConsole)
					{
						m_GameState = GAMESTATE_CONSOLE;
					}
					//else
						bSwitchToConsole = true;
					break;

				case sf::Key::M:
					m_ShowMap =! m_ShowMap;
					break;
				}
				break;
			case sf::Event::Closed:
				m_GameState = GAMESTATE_QUIT;
				break;
			case sf::Event::MouseButtonReleased:
				CGUI::GetSingleton()->MouseRelease (Event);
				break;
			}
			break;

		case GAMESTATE_CONSOLE:
			bool SwitchFlag = false;
			switch (Event.Type)
			{
			case sf::Event::KeyPressed:
				if (Event.Key.Code == sf::Key::Tilde)
					SwitchFlag = true;
				CGUI::GetSingleton()->KeyPress (Event);
				break;
			case sf::Event::TextEntered:
				CGUI::GetSingleton()->TextEnter (Event);
				break;
			case sf::Event::Closed:
				m_GameState = GAMESTATE_QUIT;
				break;
			case sf::Event::MouseButtonReleased:
				CGUI::GetSingleton()->MouseRelease (Event);
				break;
			case sf::Event::MouseButtonPressed:
				CGUI::GetSingleton()->MousePress (Event);
				break;
			case sf::Event::MouseMoved:
				CGUI::GetSingleton()->MouseMove (Event);
				break;
			}
			if (SwitchFlag)
			{		
				//ESS: poprawny stan
				m_GameState = GAMESTATE_SERVER;
				bSwitchToConsole = false;
				break;
			}
			break;
		}
	}*/
}
//======================================================================
void CGame::Go ()
{
	/*while (m_GameState)
	{
		_ResolveEvents();
		switch (m_GameState)
		{
			case GAMESTATE_LOAD:
			{
				if (m_IsServer)
					m_GameState = GAMESTATE_SERVER;
				else
					m_GameState = GAMESTATE_SPLASH;
				break;
			}

			case GAMESTATE_SERVER:
			{
				m_LuaVM.Execute("return G:Update()");
				GetLuaUpdateData();
				#pragma region Rysowanie
				CGraphics::GetSingleton()->Clear(0.0f, 0.0f, 0.0f);

				if (m_World->GetLocalPlayer())
					CGraphics::GetSingleton()->TranslateLocalView(-m_World->GetLocalPlayer()->GetPosition().X+8.0f, -m_World->GetLocalPlayer()->GetPosition().Y+5.0f);

				m_World->Draw ();
				CPhysics::GetSingleton()->DebugDraw();

				const sf::Input& Input = CGraphics::GetSingleton()->GetWindow().GetInput();
				CVector2<float> MPos(Input.GetMouseX(), Input.GetMouseY());
				if (m_ShowMap)
					m_World->DrawMap(-MPos.X/1280*360.0f, -MPos.Y/800*360.0f);

				CGUI::GetSingleton()->Draw ();
				CGUI::GetSingleton()->DrawDebug();
				CGUI::GetSingleton()->DrawConsole();

				CGraphics::GetSingleton()->Flush();
				#pragma endregion

				#pragma region Wyswietlenie FPS
				CGUI::GetSingleton()->SetDebugField ("FPS", 1/CGraphics::GetSingleton()->GetLastFrameTime ());
				CGUI::GetSingleton()->SetDebugField ("FPS2", 1/m_FrameClock.GetElapsedTime());
				#pragma endregion

				m_FrameClock.Reset();
				//CServer::GetSingleton()->Go ();
				break;
			}

			// Gra oddaje calkowicie kontrole klasie CGUI. Sluzy do wyswietlania pelnoekranowych
			// powiadomien, i takich tam. Glownie chodzi o to, zeby nie sprawdzac wejscia.
			case GAMESTATE_SPLASH:
			{
				CGraphics::GetSingleton()->Clear(0.0f, 0.0f, 0.0f);
				// Jeœli funkcja DisplaySplash zwróci true, to znacy, ¿e mo¿emy ju¿ 
				// pozwoliæ u¿ytkownikowi na wprowadzanie danych; gra przechodzi do
				// nastêpnej fazy
				if (CGUI::GetSingleton ()->DisplaySplash (CGraphics::GetSingleton()->GetLastFrameTime()))
					m_GameState = GAMESTATE_CONSOLE;
				CGraphics::GetSingleton()->Flush();
				break;
			}

			case GAMESTATE_MENU:
			{
				//CServer::GetSingleton()->Go ();
				break;
			}

			case GAMESTATE_PLAY:
			{
				sf::Clock Profiler;
				Profiler.Reset();
				float ProfileTime;
				#pragma region Wysylanie po sieci pozycji gracza
				if (m_World->GetLocalPlayer())
				{
					static CVector2<float> Pos = m_World->GetLocalPlayer()->GetPosition();

					CGUI::GetSingleton()->SetDebugField("Px", Pos.X);
					CGUI::GetSingleton()->SetDebugField("Py", Pos.Y);
					if (CNetwork::GetSingleton()->GetState())
					{
						//ESS:
						if (Pos != m_World->GetLocalPlayer()->GetPosition())
						{
							Pos = m_World->GetLocalPlayer()->GetPosition();
							static int PPPs = 0;
							CGUI::GetSingleton()->SetDebugField ("PPPs sent", ++PPPs);
							CPlayerPositionPacket TempPck (m_World->GetLocalPlayer()->GetId(), 
								Pos.X, Pos.Y, m_World->GetLocalPlayer()->GetRotation());
							CNetwork::GetSingleton()->QueryDataForSend (TempPck);
						}
					}
				}
				#pragma endregion

				CGuiGraph GraphTemplate;
				GraphTemplate.SetXStep (1);
				GraphTemplate.SetSize (CVector2<int32>(500, 300));
				GraphTemplate.SetPosition (CVector2<int32>(10, 300));
				GraphTemplate.SetBounds (CVector2<double>(0, 0.1));
				static CGuiGraph* ProfileGraph = CGUI::GetSingleton()->AddControl<CGuiGraph> (GraphTemplate);

				CNetwork::GetSingleton()->Update();

				static float GameDt = DESIRED_DELTA;
				static float PhysicsDt = 0.0f;
				PhysicsDt += GameDt;
				while (PhysicsDt > DESIRED_DELTA)
				{
					CPhysics::GetSingleton()->Update (DESIRED_DELTA);
					PhysicsDt -= DESIRED_DELTA;
				}
				m_World->Update (GameDt);
				m_LuaVM.Execute("G:Update()");
					
				ProfileTime = Profiler.GetElapsedTime();
				CGUI::GetSingleton()->SetDebugField("Network", ProfileTime);
				Profiler.Reset();

				//m_World->m_ParticleSystem.SetPos (m_World->m_Player->m_Pos.X, m_World->m_Player->m_Pos.Y);
				//m_World->m_ParticleSystem.Update();

				//CGUI::GetSingleton()->SetDebugField("MD5 test", MD5("ZERG OM NOM NOM").hexdigest());

				#pragma region Rysowanie
				CGraphics::GetSingleton()->Clear(0.0f, 0.0f, 0.0f);

				if (m_World->GetLocalPlayer())
					CGraphics::GetSingleton()->TranslateLocalView(-m_World->GetLocalPlayer()->GetPosition().X+8.0f, -m_World->GetLocalPlayer()->GetPosition().Y+5.0f);

				m_World->Draw ();
				CPhysics::GetSingleton()->DebugDraw();

				const sf::Input& Input = CGraphics::GetSingleton()->GetWindow().GetInput();
				CVector2<float> MPos(Input.GetMouseX(), Input.GetMouseY());
				if (m_ShowMap)
					m_World->DrawMap(-MPos.X/1280*360.0f, -MPos.Y/800*360.0f);

				CGUI::GetSingleton()->Draw ();
				CGUI::GetSingleton()->DrawDebug();

				CGraphics::GetSingleton()->Flush();

				ProfileTime = Profiler.GetElapsedTime();
				CGUI::GetSingleton()->SetDebugField("Drawing", ProfileTime);
				ProfileGraph->AddData (ProfileTime);
				Profiler.Reset();
				#pragma endregion

				#pragma region Wyswietlenie FPS
				CGUI::GetSingleton()->SetDebugField ("FPS", 1/CGraphics::GetSingleton()->GetLastFrameTime ());
				CGUI::GetSingleton()->SetDebugField ("FPS2", 1/m_FrameClock.GetElapsedTime());
				#pragma endregion
				GameDt = m_FrameClock.GetElapsedTime();
				m_FrameClock.Reset();
				break;
			}
			case GAMESTATE_CONSOLE:
			{
				CNetwork::GetSingleton()->Update();
				CGraphics::GetSingleton()->Clear(0.0f, 0.0f, 0.0f);
				//glColor4ub (60, 255, 34, 255);
				CGUI::GetSingleton()->Draw ();
				CGUI::GetSingleton()->DrawConsole();
				CGraphics::GetSingleton()->Flush();
				m_FrameClock.Reset();
				break;
			}
			case GAMESTATE_QUIT:
			{
				if (CNetwork::GetSingleton()->GetState())
				{
					CPlayerJoinPacket Temp;
					Temp.PlayerId = CNetwork::GetSingleton()->GetMyId();
					CNetwork::GetSingleton()->QueryDataForSend (Temp);
					CNetwork::GetSingleton()->Flush();
				}
				m_GameState = GAMESTATE_DOWN;
			}
		}
	}*/
}
//-------------------------------------------------------
void CGame::ReceivePacket (int16 packetType, const char* data)
{
	/*switch (packetType)
	{
	case EPacketType::PLAYER_POSITION: {
			CPlayerPositionPacket Pck (data);
			if (!ASS(m_World->m_NetworkPlayers.empty(), "WARNING! Server missent PPP - no network players on local"))
			{
				CPlayer* TempPlayer = m_World->m_NetworkPlayers.Get(Pck.PlayerId);
				if (TempPlayer != 0)
				{
					TempPlayer->SetPosition (CVector2<float>(Pck.X, Pck.Y));
					TempPlayer->TurnAbs (Pck.Dir);
				}
				else throw EXCEPTION("Network", "Packet sent for nonexisting player");
			}
			break;
		}

	case EPacketType::MESSAGE:
	//	Pos += Pck.Size;
		//CGUI::GetSingleton()->Message (Buf+4);
		break;

	case EPacketType::BULLET_SPAWN: {
			CBulletSpawnPacket Pck (data);
			//m_ParentGame->GetWorld()->m_ParticleSystem.Spawn (CParticle(Pck.X, Pck.Y, 1500.0f, Pck.Dir, 2.0f));
			//m_World->GetShotManager()->Shoot(m_World->m_NetworkPlayers.Get(Pck.PlayerId));
			break;
		}

	case EPacketType::PLAYER_JOIN: {
			CPlayerJoinPacket Pck (data);
						
			CPlayer* Temp = new CPlayer(Pck.PlayerId, m_World);
			m_World->m_NetworkPlayers.Add (Pck.PlayerId, Temp);
			if (!m_World->GetLocalPlayer())
			{
				CNetwork::GetSingleton()->SetMyId(Pck.PlayerId);
				m_World->CreateLocalPlayer();
			}
			else
			{
				CGUI::GetSingleton()->Message ("Player joined. Pandora minions grow stronger.");
				LOG ("Player joined.");
			}
			break;
		}
	default:
		CGUI::GetSingleton()->Message ("Unknown packet type : "+boost::lexical_cast<string,int16>(packetType));
	}*/
}
//-------------------------------------------------------
void CGame::Destroy ()
{
	/*if (m_IsServer)
		CServer::GetSingleton()->Stop();*/
	CLogger::GetSingleton()->Disable();
}
//-------------------------------------------------------
float CGame::GetLastFrameTime()
{
	return m_FrameClock.elapsed().user * 0.000001f; 
}
//-------------------------------------------------------
// pobiera z tablicy na górze stosu wartoœæ o kluczu K
#define GETTABLE(K) m_LuaVM.Push(K);lua_gettable(m_LuaVM, -2);
#define GETNUMBER(K,V) GETTABLE(K) V=m_LuaVM.PopNumber();
void CGame::GetLuaUpdateData ()
{
	string Temp = m_LuaVM.DumpStack();
	// wstawiamy na stos indeks w tablicy - 0 zawiera rozmiar
	lua_pushnumber (m_LuaVM, 0);
	// wykonujemy gettable dla -2, bo tam znajduje siê tablica z danymi
	lua_gettable (m_LuaVM, -2);
	// wartoœæ jest teraz na górze stosu
	int Count = m_LuaVM.PopNumber();
	for (int i = 1; i <= Count; ++i)
	{
		GETTABLE(i)
		{
			int Id, State;
			CVector2<float> Pos, Size;
			GETNUMBER("id", Id)
			//GETNUMBER("type", Type)
			GETTABLE("type")
			string Type = m_LuaVM.PopCString();

			GETNUMBER("state", State)

			GETTABLE("position")
			{
				Pos = m_LuaVM.GetVector2();
				m_LuaVM.Pop();
			}
			GETTABLE("size")
			{
				Size = m_LuaVM.GetVector2();
				m_LuaVM.Pop();
			}
			
			CGameObject* Object = m_World->GetObjectById(Id);
			if (!Object)
			{
				Object = new CGameObject(Id, Type, 1);
				m_World->AddObject(Object);
			}
			//ESS:
			//Object->SetPosition(Pos);
			//Object->SetSize(Size);
			Object->SetAnimationState(State);
		}
		m_LuaVM.Pop();
		/*int Type = m_LuaVM.GetNumber(); m_LuaVM.Pop();
		switch (Type)
		{
		case 1: // update obiektu
			int ObjId = m_LuaVM.GetNumber(); m_LuaVM.Pop();
			break;
		}*/
	}
	m_LuaVM.Pop(); // Zdejmujemy tablicê z danymi od Lua
}
//-------------------------------------------------------
#define PUSHSTRING(K,V) lua_pushstring(lua,K);lua_pushstring(lua,V);lua_settable(lua,-3);
#define PUSHNUMBER(K,V) lua_pushstring(lua,K);lua_pushnumber(lua,V);lua_settable(lua,-3);
#define PUSHINTEGER(K,V) lua_pushstring(lua,K);lua_pushinteger(lua,V);lua_settable(lua,-3);
int CGame::LuaGetUpdateData (lua_State* lua)
{
	uint32 Sz = m_UpdateData.size();
	// Pushujemy tabelkê, która bêdzie zawieraæ dane
	lua_createtable(lua, Sz, 1);
	uint32 i = 1;
	// Najpierw przekazujemy informacje dotycz¹ce czasu.
	//ESS: PUSHNUMBER("time", CGraphics::GetSingleton()->GetLastFrameTime());

	for (auto It = m_UpdateData.begin(); It != m_UpdateData.end(); It++)
	{
		// Ka¿da UpdateData jest tabelk¹
		lua_newtable(lua);

		switch (It->Type)
		{
		case EUpdateDataType::PLAYER_MOVEMENT:	
			PUSHSTRING("type", "unit_movement")
			PUSHSTRING("name", It->Params[0].String.c_str())
			PUSHNUMBER("pos_x", 5);
			PUSHNUMBER("pos_y", 3);
			break;
		case EUpdateDataType::MOB_DIE:
			PUSHSTRING("type", "mob_die")
			PUSHINTEGER("name", It->Params[0].Number.Int)
			//PUSHSTRING("killer", It->Params[1].String.c_str())
			break;
		case EUpdateDataType::TEST:
			PUSHSTRING("type", "test")
			PUSHSTRING("foo", It->Params[0].String.c_str())
			break;
		default:
			_CrtDbgBreak();
			break;
		}
		// Wpisujemy nowo stworzon¹ tabelkê z UpdateData
		// do zwracanej tabelki pod indeks i++
		lua_rawseti(lua, -2, i++);
	}
	m_UpdateData.clear();
	return 1;
}
//-------------------------------------------------------
int CGame::LuaGetGameVariable (lua_State* lua)
{
	if (lua_isstring (lua, -1))
	{
		const string Name (lua_tostring(lua,-1));
		if (Name == "map_time")
		{
			lua_pushnumber (lua, m_GameClock.elapsed().wall*0.000001f);
		}
		else if (Name == "version")
		{
			lua_pushstring (lua, "0.1.70");
		}
		else
			lua_pushnil(lua);
	}
	else
		lua_pushnil(lua);
	return 1;
}
//-------------------------------------------------------
CGame::CGame()
{
	m_GameState = GAMESTATE_LOAD;
	m_ShowMap = false;
	m_World=0;
}

CGame::~CGame(void)
{
}

//-------------------------------------------------------
std::vector<SUpdateData>	CGame::m_UpdateData;
