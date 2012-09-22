/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxset.h
#ifndef _FBXSDK_CORE_BASE_SET_H_
#define _FBXSDK_CORE_BASE_SET_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

#define FBXSDK_SET_DEFAULT_ITEM_COUNT 20

struct SSet;

/** Class to manipulate set
* \nosubgrouping
*/
class FBXSDK_DLL FbxSet
{
public:
	/**
	  * \name Constructors and Destructor
	  */
	//@{
		/** Int constructor.
		  * \param pItemPerBlock The number of items that every block included
		  */
		FbxSet(int pItemPerBlock=FBXSDK_SET_DEFAULT_ITEM_COUNT);

		/** Copy constructor.
		  * \param other Given object.
		  */
		FbxSet(const FbxSet& other);

		//! Destructor.
		~FbxSet();
	//@}

    // Add and remove
	/** If can't find the matching item,append a item at the end of the array.
	* If find the matching item ,insert the new item before the matching item. 
    * \param pReference The value of Reference in new item, also is the character for matching.
	* \param pItem The value of Item in new item.
	* \return If add successfully return true,otherwise return false.
    */
    bool  Add		( FbxHandle pReference, FbxHandle pItem );

	
	/** Remove the first matching item, whose reference is the same as given.
	* \param pReference The given reference.
	* \return If remove successfully return true,otherwise return false.
	*/
    bool  Remove		( FbxHandle pReference );
	
	/** Remove all the matching item, whose item is the same as given.
	* \param pItem The given item.
	* \return If remove successfully return true,otherwise return false.
	*/
    bool  RemoveItem	( FbxHandle pItem );

    /** Set first matching item with the given parameter.
    * \param pReference The character for matching.
	* \param pItem  The value of Item that the matching item will be set.
	* \return If set successfully return true,otherwise return false.
    */
    bool  SetItem	( FbxHandle pReference, FbxHandle pItem ); // Change Item Reference, don't Create if doesn't Exist

    /** Get first matching item with the given parameter.
    * \param pReference The character for matching.
	* \param pIndex The pointer to the index of the matching item.
	* \return The value of Item in the matching item.
    * \remarks If there are multiple elements that match the character, the index returned is unspecified.
    */
    FbxHandle Get ( FbxHandle pReference, int* pIndex = NULL ) const;

	//! Delete the array.
    void	 Clear();

    // Index manipulation
	/** Get the item of the given index.
    * \param pIndex The index for matching.
	* \param pReference The pointer to the Reference of the matching item.
	* \return The value of Item in the matching item.
    */
    FbxHandle GetFromIndex ( int pIndex, FbxHandle* pReference = NULL )const;

	/** Remove the item of the given index
	* \param pIndex The given index.
	* \return If remove successfully return true,otherwise return false.
	*/
    bool  RemoveFromIndex( int pIndex );

    // Get The Count
	/** Get number of items in the array.
	* \return The number of items in the array.
	*/
    int	 GetCount ()const		{ return mSetCount; }

    // Sorting
	/** Swap the value of Reference and Item in every item of array,
	 *  and sort the new array with the value of Reference. 
	 * \return If swap successfully return true,otherwise return false.
	 */
    bool  Swap()const;

	//The array can be sorted only if the private member:mIsChanged be true.
	/** Sort the array according the value of Reference in each item.
	* \return If sort successfully return true,otherwise return false.
	*/
    bool  Sort()const;

    //! FbxString assignment operator.
    const FbxSet& operator=(const FbxSet&);

private:
    // internal functions for Sets manipulation
    SSet*	FindEqual( FbxHandle pReference)const;

private:
    SSet*	mSetArray;
    int		mSetCount;
    int		mBlockCount;
    int		mItemPerBlock;
    mutable bool	mIsChanged;
};

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_CORE_BASE_SET_H_ */
