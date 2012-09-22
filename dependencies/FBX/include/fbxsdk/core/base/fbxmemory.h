/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxmemory.h
#ifndef _FBXSDK_CORE_BASE_MEMORY_H_
#define _FBXSDK_CORE_BASE_MEMORY_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

///////////////////////////////////////////////////////////////////////////////
//  WARNING!
//  Anything beyond these lines may not be documented accurately and is
//  subject to change without notice.
///////////////////////////////////////////////////////////////////////////////
#ifndef DOXYGEN_SHOULD_SKIP_THIS

class FBXSDK_DLL FbxPointerFreeGuard
{
public:
	FbxPointerFreeGuard(void* pPtr=NULL);
	virtual ~FbxPointerFreeGuard();

	void SetPointer(void* pPtr);

private:
	void* mPtr;
};

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

/** \brief Class to create a simple fixed-size-blocks memory pool to allocate memory dynamically.
  */
class FBXSDK_DLL FbxMemoryPool
{
public:
	/** The class constructor.
	  * \param pBlockSize
	  * \param pBlockCount
	  * \param pAlignment
	  * \param pResizable
	  * \param pSupportConcurrentAccess Whether the pool supports concurrent allocation and release operations.
	  */
	FbxMemoryPool(size_t pBlockSize=0, size_t pBlockCount=0, bool pAlignment=false, bool pResizable=true, bool pSupportConcurrentAccess=true);
	virtual ~FbxMemoryPool();

	/** Initiate the memory pool to the given parameters.
	  * \param pBlockSize
	  *	\param pBlockCount The blocks size.
	  *	\param pAlignment Alignment.
	  *	\param pResizable Is it resizable?
	  */
	void Init(size_t pBlockSize, size_t pBlockCount=0, bool pAlignment=false, bool pResizable=true);

	/** Uninitialized memory pool. */
	void Clear();

	/** Allocate or lock for usage a memory block.
	  *	\return a memory block.
	  */
	void* Allocate();

	/** Dispose or unlock the usage of a memory block.
	  *	Asserting that this pool own this block.
	  *	\param pMemBlock 
	  */
	void Free(void* pMemBlock);

private:
    void		InitVars();
    void*		Pop();

    FbxAtomic	mCountFreeBlock;
    void*		mFreeBlocks;
    size_t		mBlockSize;
    bool		mAlignment;
    bool		mResizable;
    bool		mSupportConcurrentAccess;
};

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_CORE_BASE_MEMORY_H_ */
