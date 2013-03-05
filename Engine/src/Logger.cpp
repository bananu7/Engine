#include <string>
#include <exception>
#include <cstdint>
#include "Logger.h"

namespace engine {

using namespace std;

void CLogger::_DebugFileOpen()
{
	if (m_LogFlags & ELogOutput::DEBUG)
	{
		m_TxtLogFile.open(m_FileName, ios::app);
	}
}

void CLogger::_DebugFileClose()
{
	if (m_LogFlags & ELogOutput::DEBUG)
		m_TxtLogFile.close();
}
//---------------------------------------------------------------------------------------------
void CLogger::Enable (uint8_t flags, const std::string& fileName)
{
	m_LogFlags = flags;
	m_FileName = fileName;
	m_TxtLogFile.open (m_FileName, ios::out);
	if (!m_TxtLogFile.is_open())
		_CrtDbgBreak();
	
	Log ("Passive Destruction Log Starting\n");
	_DebugFileClose();
}

void CLogger::Disable (void)
{
	
}
//---------------------------------------------------------------------------------------------
void CLogger::Log(const string& text)
{
	_DebugFileOpen();
	m_TxtLogFile << "[INFO] \t" << text;
	_DebugFileClose();
}

bool CLogger::Log (const string& text, bool expr, bool enforce)
{
	_DebugFileOpen();
	if (expr) m_TxtLogFile << "[FAIL] \t";
	else if (enforce) m_TxtLogFile <<"[ OK ] \t";
	m_TxtLogFile << "[INFO] \t" << text;
	_DebugFileClose();
	return expr;
}

//---------------------------------------------------------------------------------------------
CLogger::CLogger(void)
{
}

CLogger::~CLogger(void)
{
}

} // namespace engine
