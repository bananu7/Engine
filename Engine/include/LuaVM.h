#pragma once
#include <string>
#include <lua.hpp>
#include <vector>
#include <functional>
#include "Option.h"

struct lua_State;

class CLuaVM
{
protected:
	lua_State *m_LuaState;

	virtual void _ErrorFunc (int errCode);

public:
	typedef int (*TLuaFunction)(lua_State*);

	bool Load (const std::string& path);
	/// Uruchamia kod z pliku
	/// @important Ka¿dy chunk jest wykonywany tylko raz! 
	/// Jeœli wykonamy Load("plik1"); Start(); Load("plik2"), 
	/// to kod w pliku 1. zostanie uruchomiony tylko raz
	void Start ();
	void ExecuteFunc (const std::string& funcName, const std::vector<std::string>& params);
	void Execute (const std::string& code);
	void RegisterFunction (TLuaFunction, const std::string& name);
	std::string DumpStack (void);

	double GetNumber (int idx = -1);
	const char* GetCString (int idx = -1);
	bool GetBoolean (int idx = -1);
	// FIXME ?
	//CVector2<float> GetVector2 (int idx = -1);

	inline double PopNumber (int idx = -1) { double Ret = GetNumber(idx); lua_pop(m_LuaState, 1); return Ret; }
	inline const char* PopCString (int idx = -1) { const char* Ret = GetCString(idx); lua_pop(m_LuaState, 1); return Ret; }
	inline bool PopBoolean (int idx = -1) { bool Ret = GetBoolean(idx); lua_pop(m_LuaState, 1); return Ret; }

	inline void Pop (int count = 1) const { lua_pop (m_LuaState, count); }
	inline void PushGlobal (const char* global) { lua_getglobal(m_LuaState, global); }

	inline void Push (const char* val) { lua_pushstring(m_LuaState, val); }
	inline void Push (const lua_Number val) { lua_pushnumber(m_LuaState, val); }
	inline void Push (const lua_Integer val) { lua_pushinteger(m_LuaState, val); }

	// implicitly act as a lua_State pointer
	inline operator lua_State*() { return m_LuaState; }
	CLuaVM();
	virtual ~CLuaVM();
};
