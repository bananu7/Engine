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
	#define BREAKPOINT() __asm { int 3 }
#endif

