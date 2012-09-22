#pragma once
#include "Typedefs.h"
#include "Singleton.h"
#include <stdarg.h>
#include <fstream>

class CException;

enum ELogOutput : uint8
{
	DEBUG	= 1,
	TXT		= 2,
	CONSOLE	= 4,
	HTML	= 8
};

class CLogger :
	public CSingleton<CLogger>
{
protected:
	uint8			m_LogFlags;
	std::string		m_FileName;
	std::ofstream	m_TxtLogFile;

	void _DebugFileOpen();
	void _DebugFileClose();
public:
	void Enable (uint8 flags, const std::string& fileName = "log.txt");
	void Disable (void);

//	/// S³u¿y do wypisywania wartoœci pojedynczych zmiennych
//	template<typename T> void Log (const std::string& name, T val);
	/// S³u¿y do wypisywania dowolnego tekstu
	void Log (const std::string& text);
	/// S³u¿y do formatowania i wypisywania dowolnego tekstu
	void Log (const char* format, va_list args);
	/// S³u¿y do testowania danego wyra¿enia
	///@param enforce Okreœla, czy jeœli expr=true, ma wypisaæ OK
	bool Log (const std::string& text, bool expr, bool enforce = true);
	/// S³u¿y do zapisywania wyj¹tków
	void Log (const CException& e);

	CLogger(void);
	~CLogger(void);
};

#define LOG CLogger::GetSingleton()->Log
#define ASS(X) CLogger::GetSingleton()->Log(#X, X, false)
