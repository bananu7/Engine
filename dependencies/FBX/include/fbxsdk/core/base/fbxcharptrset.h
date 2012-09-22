/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxcharptrset.h
#ifndef _FBXSDK_CORE_BASE_CHARPTRSET_H_
#define _FBXSDK_CORE_BASE_CHARPTRSET_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

#define FBXSDK_CHARPTRSET_SIZE 20

/** This class contains the data structure support for char pointer set.
  */
class FBXSDK_DLL FbxCharPtrSet
{
public:
	struct CharPtrSet{ const char* mReference; FbxHandle mItemRef; };

	/** Class constructor
	*	\param pItemPerBlock Number of item per block. Default is 20.
	  */
	FbxCharPtrSet(int pItemPerBlock=FBXSDK_CHARPTRSET_SIZE);

	/** Class destructor
	  */	    
	~FbxCharPtrSet();

	/** Add a new item.
	  *	\param pReference char pointer reference to the item.
	  *	\param pItem FbxHandle to the item.
	  */
    void Add(const char* pReference, FbxHandle pItem);

	/** Removes an item.
	  *	\param pReference char reference to the item.
	  * \return true if successful.
	  */
	bool Remove(const char* pReference);

	/** Get an item's reference.
	  *	\param pReference char reference to the item.
	  *	\param PIndex index to the item.
	  * \return FbxHandle to the item, NULL if fails.
	  */
    FbxHandle Get(const char* pReference, int* PIndex=NULL);

	/** Get an item's reference from index.
	  *	\param pIndex index to the item.
	  * \return FbxHandle to the item, NULL if fails.
	  */
    FbxHandle& operator[](int pIndex);

	/** Get an item's reference from index.
	  *	\param pIndex index to the item.
	  *	\param pReference char reference to the item.
	  * \return FbxHandle to the item, NULL if fails.
	  */
    FbxHandle GetFromIndex(int pIndex, const char** pReference=NULL);

	/** Removes an item by index.
	  *	\param pIndex index to the item.
	  */
    void RemoveFromIndex(int pIndex);

	/** Get the number of item in the array.
	  */
    inline int GetCount() const { return mCharPtrSetCount; }

	/** Sorts the array.
	  */        
	void Sort();

	/** Clears the array.
	  */
    void Clear();

#ifndef DOXYGEN_SHOULD_SKIP_THIS
private:
    // internal functions for CharPtrSets manipulation
    CharPtrSet*			FindEqual( const char* pReference) const;
    CharPtrSet*			mCharPtrSetrray;
    int					mCharPtrSetCount;
    int					mBlockCount;
    int					mItemPerBlock;
    bool				mIsChanged;
    bool				mIsCaseSensitive;

public:
    inline void			SetCaseSensitive(bool pIsCaseSensitive) {mIsCaseSensitive=pIsCaseSensitive;}
#endif
};

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_CORE_BASE_CHARPTRSET_H_ */
