#pragma once
#include <cstdint>
#include <fstream>

namespace engine {

enum ELogOutput : uint8_t
{
	DEBUG	= 1,
	TXT		= 2,
	CONSOLE	= 4,
	HTML	= 8
};

class CLogger
{
protected:
	uint8_t			m_LogFlags;
	std::string		m_FileName;
	std::ofstream	m_TxtLogFile;

	void _DebugFileOpen();
	void _DebugFileClose();
public:
	void Enable (uint8_t flags, const std::string& fileName = "log.txt");
	void Disable (void);

//	template<typename T> void Log (const std::string& name, T val);
	void Log (const std::string& text);
	bool Log (const std::string& text, bool expr, bool enforce = true);

	CLogger(void);
	~CLogger(void);
};

} // namespace engine