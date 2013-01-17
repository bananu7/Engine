
#include "LuaVM.h"

using namespace std;

bool CLuaVM::Load (const string& path)
{
	if (luaL_loadfile (m_LuaState, path.c_str()) == 0)
		return true;
	else
		return false;
}

void CLuaVM::_ErrorFunc (int errCode)
{
	if (errCode != 0)
	{
		std::string Error = "Lua error : ";
		Error += lua_tostring(m_LuaState, -1);
		//TODO:
		//CGUI::GetSingleton()->Message(Error);
		lua_pop(m_LuaState, 1); // remove error message
	}
}

//void CLuaVM::Execute (const std::string& funcName, const std::vector<SOption>& params)
void CLuaVM::Start ()
{
	// Wywo³anie g³ównego programu skryptu
	_ErrorFunc (lua_pcall(m_LuaState, 0, LUA_MULTRET, 0));
}

void CLuaVM::ExecuteFunc (const string& funcName, const vector<string>& params)
{
	// £adujemy funckjê
	lua_getglobal (m_LuaState, funcName.c_str());
	int p = lua_gettop(m_LuaState);
	// £adujemy parametry
	for (auto It = params.begin(); It != params.end(); It++)
		lua_pushstring (m_LuaState, It->c_str());
	
	// Wywo³anie
	_ErrorFunc (lua_pcall(m_LuaState, params.size(), LUA_MULTRET, 0));
}

void CLuaVM::Execute (const string& code)
{
	// £adujemy kod
	_ErrorFunc(luaL_loadstring (m_LuaState, code.c_str()));
	Start();
//	int Ret3 = lua_gettop (m_LuaState);
}

void CLuaVM::RegisterFunction (TLuaFunction func, const std::string& name)
{
//	lua_pushcfunction (m_LuaState, func.target<int(lua_State*)>());
	lua_pushcfunction (m_LuaState, func);
	lua_setglobal (m_LuaState, name.c_str());
}

string CLuaVM::DumpStack (void)
{
	string Ret;
	int i;
	int top = lua_gettop(m_LuaState);
	for (i = 1; i <= top; i++)
	{  /* repeat for each level */
		int t = lua_type(m_LuaState, i);
		switch (t) 
		{
			/*case LUA_TSTRING:
			printf("`%s'", lua_tostring(L, i));
			break;
	
			case LUA_TBOOLEAN:
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;
	
			case LUA_TNUMBER:
			printf("%g", lua_tonumber(L, i));
			break;*/
	
			default:  /* other values */
			//printf("%s", lua_typename(L, t));
			Ret += lua_typename (m_LuaState, t);
			Ret += ' ';
			break;
		}
	}
	return Ret;
}

double CLuaVM::GetNumber (int idx)
{
	if (lua_isnumber(m_LuaState, idx))
		return lua_tonumber(m_LuaState, idx);
	else _CrtDbgBreak(); 
}
const char* CLuaVM::GetCString (int idx)
{
	if (lua_isstring(m_LuaState, idx))
		return lua_tostring(m_LuaState, idx);
	else _CrtDbgBreak();
}
bool CLuaVM::GetBoolean (int idx)
{
	if (lua_isboolean(m_LuaState, idx))
		return lua_toboolean(m_LuaState, idx);
	else _CrtDbgBreak();
}

CVector2<float> CLuaVM::GetVector2 (int idx)
{
	CVector2<float> Ret;
	lua_pushstring (m_LuaState, "x");
	lua_gettable (m_LuaState, -2);
	Ret.X = PopNumber();
	lua_pushstring (m_LuaState, "y");
	lua_gettable (m_LuaState, -2);
	Ret.Y = PopNumber();
	return Ret;
}

CLuaVM::CLuaVM(void) :
	m_LuaState(lua_open()) 
{
	luaL_openlibs(m_LuaState);
}

CLuaVM::~CLuaVM(void)
{
	lua_close(m_LuaState);
}
