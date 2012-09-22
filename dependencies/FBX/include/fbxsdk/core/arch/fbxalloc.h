/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

/** \file fbxalloc.h
  * Allocation functions definition.
  *
  * It is possible to override memory allocation functions throughout the FBX SDK by
  * providing system memory allocation functions using the handler set functions below.
  * The Microsoft Windows implementation in debug mode allows to specify where the
  * allocations happen by providing the standard block type, file name and line number.
  */
#ifndef _FBXSDK_CORE_ARCH_ALLOC_H_
#define _FBXSDK_CORE_ARCH_ALLOC_H_

#include <fbxsdk/fbxsdk_def.h>

#if defined(_DEBUG) && defined(FBXSDK_ENV_WIN)
	#include <crtdbg.h>
#endif

#if defined(FBXSDK_ENV_MAC)
	#include <malloc/malloc.h>
#else
	#include <malloc.h>
#endif

#include <fbxsdk/fbxsdk_nsbegin.h>

#if defined(FBXSDK_ARCH_32)
	#define FBXSDK_MEMORY_ALIGNMENT 4
#elif defined(FBXSDK_ARCH_64)
	#define FBXSDK_MEMORY_ALIGNMENT 8
#endif

#ifndef _msize
	#if defined(FBXSDK_ENV_MAC)
		#define _msize ((size_t(*)(void*))malloc_size)
	#elif defined(FBXSDK_ENV_LINUX)
		#define _msize ((size_t(*)(void*))malloc_usable_size)
	#endif
#endif

#if defined(_DEBUG) && defined(FBXSDK_ENV_WIN)
	typedef void*	(*FbxMallocProc)(size_t, int, const char*, int);			//! Function pointer signature used to replace "malloc" in debug mode
	typedef void*	(*FbxCallocProc)(size_t, size_t, int, const char*, int);	//! Function pointer signature used to replace "calloc" in debug mode
	typedef void*	(*FbxReallocProc)(void*, size_t, int, const char*, int);	//! Function pointer signature used to replace "realloc" in debug mode
	typedef void	(*FbxFreeProc)(void*, int);									//! Function pointer signature used to replace "free" in debug mode
	typedef size_t	(*FbxMSizeProc)(void*, int);								//! Function pointer signature used to replace "_msize" in debug mode
#else
	typedef void*	(*FbxMallocProc)(size_t);									//! Function pointer signature used to replace "malloc" in release mode
	typedef void*	(*FbxCallocProc)(size_t, size_t);							//! Function pointer signature used to replace "calloc" in release mode
	typedef void*	(*FbxReallocProc)(void*, size_t);							//! Function pointer signature used to replace "realloc" in release mode
	typedef void	(*FbxFreeProc)(void*);										//! Function pointer signature used to replace "free" in release mode
	typedef size_t	(*FbxMSizeProc)(void*);										//! Function pointer signature used to replace "_msize" in release mode
#endif

/** Set the global memory allocation function used internally by the FBX SDK.
  * \param pHandler Function pointer that implements the necessary procedure to allocate memory in the system.
  */
FBXSDK_DLL void FbxSetMallocHandler(FbxMallocProc pHandler);

/** Set the global zero'd memory allocation function used internally by the FBX SDK.
  * \param pHandler Function pointer that implements the necessary procedure to allocate zero'd memory in the system.
  */
FBXSDK_DLL void FbxSetCallocHandler(FbxCallocProc pHandler);

/** Set the global memory re-allocation function used internally by the FBX SDK.
  * \param pHandler Function pointer that implements the necessary procedure to re-allocate memory in the system.
  */
FBXSDK_DLL void FbxSetReallocHandler(FbxReallocProc pHandler);

/** Set the global memory freeing function used internally by the FBX SDK.
  * \param pHandler Function pointer that implements the necessary procedure to free memory in the system.
  */
FBXSDK_DLL void FbxSetFreeHandler(FbxFreeProc pHandler);

/** Set the global memory size retrieval function used internally by the FBX SDK.
  * \param pHandler Function pointer that implements the necessary procedure to retrieve memory allocation size in the system.
  */
FBXSDK_DLL void FbxSetMSizeHandler(FbxMSizeProc pHandler);

//Internal use only
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	FBXSDK_DLL void* FbxMalloc(size_t pSize);
	FBXSDK_DLL void* FbxCalloc(size_t pCount, size_t pSize);
	FBXSDK_DLL void* FbxRealloc(void* pData, size_t pSize);
	FBXSDK_DLL void FbxFree(void* pData);
	FBXSDK_DLL size_t FbxMSize(void* pData);
	FBXSDK_DLL void* FbxMallocAligned(size_t pSize);
	FBXSDK_DLL void* FbxCallocAligned(size_t pCount, size_t pSize);
	FBXSDK_DLL void* FbxReallocAligned(void* pAlignedPtr, size_t pSize);
	FBXSDK_DLL void FbxFreeAligned(void* pAlignedPtr);
	FBXSDK_DLL char* FbxStrDup(const char* pString);
	FBXSDK_DLL wchar_t* FbxStrDupWC(const wchar_t* pString);

	#if defined(_DEBUG) && defined(FBXSDK_ENV_WIN)
		FBXSDK_DLL void* FbxMallocDebug(size_t pSize, int pBlock, const char* pFile, int pLine);
		FBXSDK_DLL void* FbxCallocDebug(size_t pCount, size_t pSize, int pBlock, const char* pFile, int pLine);
		FBXSDK_DLL void* FbxReallocDebug(void* pData, size_t pSize, int pBlock, const char* pFile, int pLine);
		FBXSDK_DLL void FbxFreeDebug(void* pData, int pBlock);
		FBXSDK_DLL size_t FbxMSizeDebug(void* pData, int pBlock);
		FBXSDK_DLL char* FbxStrDupDebug(const char* pString, int pBlock, const char* pFile, int pLine);
		FBXSDK_DLL wchar_t* FbxStrDupWCDebug(const wchar_t* pString, int pBlock, const char* pFile, int pLine);

		#define FbxMalloc(s) FbxMallocDebug(s, _NORMAL_BLOCK, __FILE__, __LINE__)
		#define FbxCalloc(c, s) FbxCallocDebug(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
		#define FbxRealloc(p, s) FbxReallocDebug(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
		#define FbxFree(p) FbxFreeDebug(p, _NORMAL_BLOCK)
		#define FbxMSize(p) FbxMSizeDebug(p, _NORMAL_BLOCK)
		#define FbxStrDup(s) FbxStrDupDebug(s, _NORMAL_BLOCK, __FILE__, __LINE__)
		#define FbxStrDupWC(s) FbxStrDupWCDebug(s, _NORMAL_BLOCK, __FILE__, __LINE__)
	#endif
#endif

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_CORE_ARCH_ALLOC_H_ */
