#pragma once

#ifdef _WIN32
	#define MINICRAFT_WINDOWS
#else
	#define MINICRAFT_LINUX
#endif

#ifdef MINICRAFT_WINDOWS
	#define BREAKPOINT() _CrtDbgBreak()
#endif

#ifdef MINICRAFT_LINUX
	// this has to stay here until I find something normal -.-
	#include <signal.h>
	#define BREAKPOINT() raise(SIGTRAP)
#endif

