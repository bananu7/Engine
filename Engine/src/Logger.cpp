#include <string>
#include <exception>
#include <cstdint>
#include "Logger.h"

namespace engine {

using namespace std;

void Logger::_DebugFileOpen()
{
	if (m_LogFlags & ELogOutput::DEBUG)
	{
		m_TxtLogFile.open(m_FileName, ios::app);
	}
}

void Logger::_DebugFileClose()
{
	if (m_LogFlags & ELogOutput::DEBUG)
		m_TxtLogFile.close();
}
//---------------------------------------------------------------------------------------------
void Logger::Enable (uint8_t flags, const std::string& fileName)
{
	m_LogFlags = flags;
	m_FileName = fileName;
	m_TxtLogFile.open (m_FileName, ios::out);
	if (!m_TxtLogFile.is_open())
		_CrtDbgBreak();
	
	Log ("Passive Destruction Log Starting\n");
	_DebugFileClose();
}

void Logger::Disable (void)
{
	
}
//---------------------------------------------------------------------------------------------
void Logger::Log(const string& text)
{
	_DebugFileOpen();
	m_TxtLogFile << "[INFO] \t" << text;
	_DebugFileClose();
}

bool Logger::Log (const string& text, bool expr, bool enforce)
{
	_DebugFileOpen();
	if (expr) m_TxtLogFile << "[FAIL] \t";
	else if (enforce) m_TxtLogFile <<"[ OK ] \t";
	m_TxtLogFile << "[INFO] \t" << text;
	_DebugFileClose();
	return expr;
}

//---------------------------------------------------------------------------------------------
Logger::Logger(void)
{
}

Logger::~Logger(void)
{
}

} // namespace engine
