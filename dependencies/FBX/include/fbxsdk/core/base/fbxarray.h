/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxarray.h
#ifndef _FBXSDK_CORE_BASE_ARRAY_H_
#define _FBXSDK_CORE_BASE_ARRAY_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

#define FBXSDK_ARRAY_BLOCKSIZE 4

template <size_t TypeSize> class FbxArrayBase
{
public:
    /** Get number of pointers in the array.
    * \return The number of items in the array.
    */
    inline int GetCount() const { return GetArrayCount(); }

    //! Remove all pointers without deleting the associated objects.
    inline void Clear()
    {
        if (mBaseArray!=NULL)
        {
            FbxFree(mBaseArray);
            mArrayCount = 0;
            mBaseArray = NULL;
        }
    }


    //! Fast empty, set object count to zero but don't free any memory.
    inline void Empty()
    {
	#ifdef _DEBUG
		memset( mBaseArray+ GetHeaderOffset() ,0,GetArrayCount()*TypeSize);
	#endif
        SetArrayCount(0);
    }


    /** Set array capacity to contain at least the specified number of elements without reallocating.
    * \param pCapacity Number of items that can be stored in the array before reallocating the memory.
    * \return The number of available slots in the array.
    * \remarks If capacity is lower than arrayCount, arrayCount is lowered to capacity.
    */
    inline int Reserve(int pCapacity)
    {
		FBX_ASSERT( pCapacity > 0 );

        if( pCapacity )
        {
            const FbxUInt lTempNewBlockCount = ( (FbxUInt) (pCapacity + FBXSDK_ARRAY_BLOCKSIZE - 1 ) / FBXSDK_ARRAY_BLOCKSIZE );
            const FbxUInt lNewBlockCount = (lTempNewBlockCount > 1 ? lTempNewBlockCount : 1);

            int lArrayCount = GetArrayCount();
            int lBlockCount = GetBlockCount();

            const FbxUInt lOldArraySize = lArrayCount*TypeSize;
            const FbxUInt lNewArraySize = lNewBlockCount*FBXSDK_ARRAY_BLOCKSIZE*TypeSize;

            if (lNewBlockCount != (FbxUInt) lBlockCount)
            {
                char* lBaseArray = (char*)FbxRealloc(mBaseArray, (size_t) lNewArraySize+ GetHeaderOffset()  );
                if (!lBaseArray)
                    return GetBlockCount()*FBXSDK_ARRAY_BLOCKSIZE;
                mBaseArray = lBaseArray;
            }

            if( lNewBlockCount > (FbxUInt) lBlockCount ) {
                memset( ((char*)mBaseArray+ GetHeaderOffset() ) + lOldArraySize, 0, (size_t) (lNewArraySize-lOldArraySize) );
                SetArrayCount(lArrayCount);
            } else if (pCapacity < lArrayCount)
            {
                memset( ((char*)mBaseArray+ GetHeaderOffset() ) + pCapacity*TypeSize, 0, (size_t) (lNewArraySize-pCapacity*TypeSize) );
                SetArrayCount(pCapacity);
            }

            SetBlockCount(lNewBlockCount);
        }

        return GetBlockCount()*FBXSDK_ARRAY_BLOCKSIZE;
    }


    //! Set arrayCount to specified number of elements. The array capacity is adjusted accordingly.
    //  Differ from SetCount because array capacity can be lowered.

    /** Force the array of elements to a given size.
    * \remarks If the array is upsized, the memory allocated is set to 0 and
    * no constructor is called. Thus, this function is not appropriate for
    * types of elements requiring initialization.
    */
    inline void SetCount (int pCount)
    {
    #ifdef _DEBUG
        if (pCount<0)
        {
            FBX_ASSERT_NOW ("ArrayUL : Item count can't be negative");
            return ;
        }
    #endif
        int lArrayCount = GetArrayCount();
        if (pCount > lArrayCount)
        {
            AddMultiple( pCount-lArrayCount);
        } 
		else
        {
            SetArrayCount(pCount);
        }
    }

    inline void Resize(int pItemCount)
    {
		FBX_ASSERT( pItemCount >= 0 );

        const FbxUInt lTempNewBlockCount = ( (FbxUInt) (pItemCount + FBXSDK_ARRAY_BLOCKSIZE - 1 ) / FBXSDK_ARRAY_BLOCKSIZE );
        const FbxUInt lNewBlockCount = (lTempNewBlockCount > 1 ? lTempNewBlockCount : 1);

        int lArrayCount = GetArrayCount();
        int lBlockCount = GetBlockCount();

        const FbxUInt lOldArraySize = lArrayCount*TypeSize;
        const FbxUInt lNewArraySize = lNewBlockCount*FBXSDK_ARRAY_BLOCKSIZE*TypeSize;

        if (lNewBlockCount != (FbxUInt) lBlockCount)
        {
            char* lBaseArray = (char*)FbxRealloc(mBaseArray, (size_t) lNewArraySize+ GetHeaderOffset()  );
            if (!lBaseArray)
                return;
            mBaseArray = lBaseArray;
        }

        if( lNewBlockCount > (FbxUInt) lBlockCount )
		{
            memset( ((char*)mBaseArray+ GetHeaderOffset() ) + lOldArraySize, 0, (size_t) (lNewArraySize-lOldArraySize) );
		}
        else if (pItemCount < lArrayCount)
		{
            memset( ((char*)mBaseArray+ GetHeaderOffset() ) + pItemCount*TypeSize, 0, (size_t) (lNewArraySize-pItemCount*TypeSize) );
		}

        SetBlockCount(lNewBlockCount);
        SetArrayCount(pItemCount);
    }

    inline void AddMultiple(int pItemCount)
    {
		FBX_ASSERT( pItemCount > 0 );

        if( pItemCount )
        {
            int lArrayCount = GetArrayCount();
            int lBlockCount = GetBlockCount();
            const FbxUInt lTempNewBlockCount = ( (FbxUInt) (lArrayCount+pItemCount + FBXSDK_ARRAY_BLOCKSIZE - 1 ) / FBXSDK_ARRAY_BLOCKSIZE );
            const FbxUInt lNewBlockCount = (lTempNewBlockCount > 1 ? lTempNewBlockCount : 1);

            const FbxUInt lOldArraySize = lArrayCount*TypeSize;
            const FbxUInt lNewArraySize = lNewBlockCount*FBXSDK_ARRAY_BLOCKSIZE*TypeSize;

			FBX_ASSERT( lOldArraySize < lNewArraySize );

            if( lNewBlockCount > (FbxUInt) lBlockCount )
            {
                char* lBaseArray = (char*)FbxRealloc(mBaseArray, (size_t) lNewArraySize+ GetHeaderOffset()  );
                if (!lBaseArray)
                    return;
                mBaseArray = lBaseArray;
                lBlockCount = lNewBlockCount;
            }

            memset( ((char*)mBaseArray+ GetHeaderOffset() ) + lOldArraySize, 0, (size_t) (lNewArraySize-lOldArraySize) );
            SetArrayCount ( lArrayCount + pItemCount );
            SetBlockCount (lBlockCount);
        }
    }

///////////////////////////////////////////////////////////////////////////////
//
//  WARNING!
//
//  Anything beyond these lines may not be documented accurately and is
//  subject to change without notice.
//
///////////////////////////////////////////////////////////////////////////////
#ifndef DOXYGEN_SHOULD_SKIP_THIS
protected:
    struct Header {
        int mBlockCount;
    };

    //! Constructor.
    inline FbxArrayBase() : mArrayCount(0), mBaseArray(NULL)
    {
    }

    //! Destructor.
    inline ~FbxArrayBase(){
        Clear ();
    }

    /** Insert an item at the given position.
    * \param pIndex Position where to insert the item.
    * \param pItem  Pointer to the item to be inserted.
    * \remarks if pIndex is greater than the number of items already in the
    * array, the item will be appended at the end.
    * \return The actual position where the item as been inserted.
    */
    inline int InsertAt(int pIndex, void *pItem)
    {
		int lArrayCount = GetArrayCount();
		int lBlockCount = GetBlockCount();

		FBX_ASSERT( pIndex >= 0 );

        if (pIndex>lArrayCount) {
            pIndex = GetArrayCount();
        }

        if (lArrayCount>= lBlockCount*FBXSDK_ARRAY_BLOCKSIZE)
        {
            // must Alloc.Realloc some new space

            // double the number of blocks.
            lBlockCount = ( 0 == lBlockCount ) ? 1 : lBlockCount * 2;
            char* lBaseArray = (char*)FbxRealloc(mBaseArray, (size_t) (lBlockCount*FBXSDK_ARRAY_BLOCKSIZE*TypeSize) + GetHeaderOffset() );
            if(!lBaseArray)
                return -1;
            mBaseArray = lBaseArray;
        }

        if (pIndex<lArrayCount)
        {
            // This is an insert
            memmove (&(mBaseArray[(pIndex+1)*TypeSize+ GetHeaderOffset() ]), &(mBaseArray[(pIndex)*TypeSize+ GetHeaderOffset()] ), TypeSize*(lArrayCount-pIndex));
        }

        memmove (&(mBaseArray[(pIndex)*TypeSize+ GetHeaderOffset() ]), pItem, TypeSize);

        SetArrayCount(lArrayCount+1);
        SetBlockCount(lBlockCount);

        return pIndex;
    }


    /** Get the item at the given position.
    * \param pIndex The position of the item to access.
    * \return Pointer to the item.
    * \remarks This method assumes that the passed index is
    * in the valid range of the array. No checks are made.
    */
    inline void* GetAt(int pIndex) { return &(mBaseArray[(pIndex)*TypeSize+ GetHeaderOffset() ]); }

    /** Removes the item at the given position.
    * \param pIndex The position of the item to remove.
    * \remarks If the index is not valid, nothing is performed. Otherwise,
    * the item is removed from the array and the items are shifted to fill the
    * empty slot.
    */
    inline void RemoveAt(int pIndex)
    {

    #if defined(_DEBUG) && !defined(FBXSDK_ENV_MAC)
        if (!ValidateIndex( pIndex ))
        {
            return;
        }
    #endif
        int lArrayCount = GetArrayCount();
        if (pIndex+1<lArrayCount)
        {
            memmove (&(mBaseArray[(pIndex)*TypeSize+ GetHeaderOffset() ]), &(mBaseArray[(pIndex+1)*TypeSize+ GetHeaderOffset() ]), TypeSize*(lArrayCount-pIndex-1));
        }

        SetArrayCount( lArrayCount-1 );

    #ifdef _DEBUG
        memset( &(mBaseArray[(GetArrayCount())*TypeSize+ GetHeaderOffset() ]),0,TypeSize);
    #endif
    }


    /** Check that the given position is inside the array boundaries.
    * \param pIndex Index value to validate.
    * \return \c true if the index value is within the array boundaries. \c false
    * otherwise.
    */
    inline bool ValidateIndex( int pIndex ) const
    {
        int lArrayCount = GetArrayCount();
        if (pIndex>=0 && pIndex<lArrayCount)
        {
            return true;
        } 
		else
        {
			FBX_ASSERT_NOW("Array : Index out of range");
            return false;
        }
    }

    inline Header* const GetHeader() const
    {
        return (Header* const)mBaseArray;
    }
    inline Header* GetHeader()
    {
        return (Header*)mBaseArray;
    }
    inline int GetHeaderOffset() const
    {
        return sizeof(Header);
    }
    inline int GetArrayCount() const
    {
        return mArrayCount;
    }
    inline void SetArrayCount(int pArrayCount)
    {
        mArrayCount = pArrayCount;
    }
    inline int GetBlockCount() const
    {
        return GetHeader() ? GetHeader()->mBlockCount : 0;
    }
    inline void SetBlockCount(int pArrayCount)
    {
        if (GetHeader()) GetHeader()->mBlockCount = pArrayCount;
    }

    int		mArrayCount;
    char*	mBaseArray;
#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS
};

/** Class for array of elements such as pointers and plain old data structures.
  * This class will not call constructor and destructor for elements.
  */
template <class Type> class FbxArray : public FbxArrayBase<sizeof(Type)>
{
    typedef FbxArrayBase<sizeof(Type)> ParentClass;

#if defined(FBXSDK_COMPILER_MSC)
    // Previously class FbxArray is for pointers. Somehow, it's used to store other types.
    // Here's a compile-time checking for known incompatible classes.
    // If it happens you find new incompatible ones, declare them with macro FBXSDK_INCOMPATIBLE_WITH_ARRAY
    // Also see file string.h
    FBX_ASSERT_STATIC(FBXSDK_IS_SIMPLE_TYPE(Type) || __is_enum(Type) || (__has_trivial_constructor(Type)&&__has_trivial_destructor(Type)) || !FBXSDK_IS_INCOMPATIBLE_WITH_ARRAY(Type));
#endif

public:
    //! Constructor.
    FbxArray() : ParentClass() {}

    //! Copy constructor.
    FbxArray(const FbxArray& pArrayTemplate) : ParentClass()
    {
        *this = pArrayTemplate;
    }

    //! Destructor. The destructor of elements will not be called.
    ~FbxArray() {}

    /** Insert a element.
      * \param pIndex Position where to insert the pointer.
      * \param pItem Item to insert.
      * \return Position of the inserted pointer in the array.
      * \remarks If the given index is out of range, the pointer is appended at the end. No error will be thrown if the index is out of bounds.
      */
    inline int InsertAt(int pIndex, Type pItem)
    {
        return ParentClass::InsertAt( pIndex,&pItem );
    }

    /** Remove a element in the array.
      * \param pIndex Position of the item to remove.
      * \return Removed item.
      * \remarks No error will be thrown if the index is out of bounds.
      */
    inline Type RemoveAt(int pIndex)
    {
        Type tmpItem = GetAt(pIndex);
        ParentClass::RemoveAt( pIndex );
        return tmpItem;
    }

    /** Remove the last element in the array.
      * \return Removed item.
      * \remarks The array should have at least one element and no error will be thrown if the array is empty.
      */
    inline Type RemoveLast()
    {
        return RemoveAt(ParentClass::GetArrayCount()-1);
    }

    /** Remove first matching element in the array.
      * \param pItem Item to be removed.
      * \return \c true if a matching pointer is found and removed, \c false otherwise.
      */
    inline bool RemoveIt(Type pItem)
    {
        int Index = Find (pItem);
        if (Index>=0)
        {
            RemoveAt (Index);
            return true;
        }
        return false;
    }

    /** Returns a reference to the element at given position in the array.
      * \param pIndex Position of element in the array.
      * \return The element at the specified position in the array.
      * \remarks No error will be thrown if the index is out of bounds.
      */
    inline Type &operator[](int pIndex) const
    {
    #if defined(_DEBUG) && !defined(FBXSDK_ENV_MAC)
        if (!ParentClass::ValidateIndex( pIndex ))
        {
            return (Type&)(ParentClass::mBaseArray[(0)*sizeof(Type)+ ParentClass::GetHeaderOffset()]);
        }
    #endif
        return (Type&)(ParentClass::mBaseArray[(pIndex)*sizeof(Type)+ ParentClass::GetHeaderOffset()]);
    }

    /** Set the element at given position in the array.
      * \param pIndex Position of element in the array.
      * \param pItem The new element.
      * \remarks No error will be thrown if the index is out of bounds.
      */
    inline void SetAt(int pIndex, Type pItem)
    {
    #if defined(_DEBUG) && !defined(FBXSDK_ENV_MAC)
        if (!ParentClass::ValidateIndex( pIndex ))
        {
            return;
        }
    #endif
        GetArray()[pIndex] = pItem;
    }

    /** Set the value of the last element.
      * \param pItem The new value of the last element.
      * \remarks The array should have at least one element and no error will be thrown if the array is empty.
      */
    inline void SetLast(Type pItem)
    {
        SetAt (ParentClass::GetArrayCount()-1, pItem);
    }

    /** Returns the value of the element at given position in the array.
      * \param pIndex Position of element in the array.
      * \return The value of the element at the specified position in the array.
      * \remarks No error will be thrown if the index is out of bounds.
      */
    inline Type GetAt(int pIndex) const
    {
    #if defined(_DEBUG) && !defined(FBXSDK_ENV_MAC)
        if (!ParentClass::ValidateIndex( pIndex ))
        {
            return (Type&)(ParentClass::mBaseArray[(0)*sizeof(Type)+ ParentClass::GetHeaderOffset()]);
        }
    #endif
        return (Type&)(ParentClass::mBaseArray[(pIndex)*sizeof(Type)+ ParentClass::GetHeaderOffset()]);
    }

    /** Get the first element.
      * \return The first element.
      * \remarks The array should have at least one element and no error will be thrown if the array is empty.
      */
    inline Type GetFirst() const
    {
        FBX_ASSERT( ParentClass::GetArrayCount() >= 1 );
        return GetAt(0);
    }

    /** Get the last element.
      * \return The last element.
      * \remarks The array should have at least one element and no error will be thrown if the array is empty.
      */
    inline Type GetLast() const
    {
        FBX_ASSERT( ParentClass::GetArrayCount() >= 1 );
        return GetAt(ParentClass::GetArrayCount()-1);
    }

    /** Find first matching element.
      * \param pItem The item to be compared to each of the elements.
      * \return Index of first matching element or -1 if there is no matching element.
      */
    inline int Find(Type pItem) const
    {
        return FindAfter( -1, pItem );
    }

    /** Find first matching element after given index.
      * \param pAfterIndex The index after which the finding begins.
      * \param pItem The item to be compared to each of the elements.
      * \return Index of first matching pointer found after given index or -1 if there is no matching pointer.
      * \remarks The index must be within valid range and no error will be thrown if it is invalid.
      */
    inline int FindAfter(int pAfterIndex, Type pItem) const
    {
    #ifdef _DEBUG
        if ( pAfterIndex > ParentClass::GetArrayCount() || pAfterIndex < -1 )
        {
            FBX_ASSERT_NOW ("ArrayUL : Search Begin Index out of range");
            return -1;
        }
    #endif
        int Count;
        for ( Count=pAfterIndex+1; Count<ParentClass::GetArrayCount(); Count++)
        {
            if (GetAt(Count)==pItem)
            {
                return Count;
            }
        }
        return -1;
    }

    /** Find first matching pointer before given index.
      * \param pBeforeIndex The index before which the finding begins.
      * \param pItem The item to be compared to each of the elements.
      * \return Index of first matching pointer found before given index or -1 if there is no matching pointer.
      * \remarks The index must be within valid range and no error will be thrown if it is invalid.
      */
    inline int FindBefore(int pBeforeIndex, Type pItem) const
    {
    #ifdef _DEBUG
        if ( pBeforeIndex > ParentClass::GetArrayCount() || pBeforeIndex <= 0 )
        {
            FBX_ASSERT_NOW ("ArrayUL : Search Begin Index out of range");
            return -1;
        }
    #endif
        int Count;
        for ( Count=pBeforeIndex-1; Count>=0; Count--)
        {
            if (GetAt(Count)==pItem)
            {
                return Count;
            }
        }
        return -1;
    }

    /** Append an element at the end of the array.
      * \param pItem Value to be copied to the new element.
      * \return Index of appended element.
      */
    inline int Add(Type pItem)
    {
        return InsertAt(ParentClass::GetArrayCount(), pItem);
    }

    /** Add Element at the end of array if not present.
      * \param pItem Value to be copied to the new element.
      * \return Index of appended element.
      */
    inline int AddUnique(Type pItem)
    {
        int lReturnIndex = Find(pItem);
        if (lReturnIndex == -1)
        {
            lReturnIndex = Add(pItem);
        }
        return lReturnIndex;
    }

    /** Add multiple elements at the end of array, use SetAt or GetArray to set the value of the new elements.
      * \param pItemCount How many new elements you want to add.
      */
    inline void AddMultiple(FbxUInt pItemCount)
    {
        ParentClass::AddMultiple(pItemCount);
    }

    /** Append another array at the end of this array.
      * \param pArray Another array.
      */
    inline void AddArray(const FbxArray<Type> &pArray)
    {
        int lSourceIndex, lCount = pArray.GetCount();
        if( lCount == 0 ) return;
        int lDestinationIndex = ParentClass::GetCount();
        AddMultiple(lCount);
        for( lSourceIndex = 0; lSourceIndex < lCount; lSourceIndex++)
        {
            SetAt(lDestinationIndex++, pArray[lSourceIndex]);
        }
    }

    /** Append the elements of another array at the end of this array if they are not present.
      * \param pArray Another array.
      */
    inline void AddArrayNoDuplicate(const FbxArray<Type> &pArray)
    {
        int i, lCount = pArray.GetCount();
        for( i = 0; i < lCount; i++)
        {
            Type lItem = pArray[i];
            if (Find(lItem) == -1)
            {
                Add(lItem);
            }
        }
    }

    /** Remove the elements of another array from this array is they are present.
      * \param pArray Another array.
      */
    inline void RemoveArray(const FbxArray<Type> &pArray)
    {
        int lRemoveIndex, lRemoveCount = pArray.GetCount();
        for( lRemoveIndex = 0; lRemoveIndex < lRemoveCount; lRemoveIndex++)
        {
            RemoveIt(pArray[lRemoveIndex]);
        }
    }

    //! Get pointer to internal array of elements.
    inline Type* GetArray() const
    {
        if (ParentClass::mBaseArray == NULL)
            return NULL;

        return (Type*)(ParentClass::mBaseArray+ ParentClass::GetHeaderOffset()) ;
    }

    //! Copy array of elements.
    inline FbxArray<Type>& operator=(const FbxArray<Type>& pArrayTemplate)
    {
        if ( this != &pArrayTemplate )
        {
            ParentClass::Clear();

            int i, lCount = pArrayTemplate.GetCount();

            for (i = 0; i < lCount; i++)
            {
                Add(pArrayTemplate[i]);
            }
        }

        return (*this);
    }

    //! Cast operator.
    inline operator Type* ()
    {
        return GetArray();
    }
};

template <class Type> inline void FbxArrayDelete(FbxArray<Type>& Array)
{
    FbxUInt lItemCount = Array.GetCount();
    while( lItemCount )
    {
        lItemCount--;
        Type& Item = (Array.operator[](lItemCount));
        FbxDelete(Item);
        Item = NULL;
    }
    Array.Clear();
}

///////////////////////////////////////////////////////////////////////////////
//
//  WARNING!
//
//  Anything beyond these lines may not be documented accurately and is
//  subject to change without notice.
//
///////////////////////////////////////////////////////////////////////////////
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	class FbxArrayBase_Size
	{
	public:
		FbxArrayBase_Size(int pItemSize) : mItemSize(pItemSize) {}
		inline int GetTypeSize() const { return mItemSize; }

	private:
		int mItemSize;
	};

	/** Template version base class used to store an array of fixed-size elements.
	  * This class is used internally and only contains interfaces used for memory management
	  * of array elements.
	  */
	template <class TypeSize> class FbxArrayBase_
	{
	protected:
		struct Header
		{
			int mArrayCount;
			int mBlockCount;
		};

		/** Constructor.
		  * \param pTypeSize Size of one item of the array.
		  */
		inline FbxArrayBase_(TypeSize pTypeSize) : mTypeSize(pTypeSize)
		{
			mBaseArray = NULL;
		}

		//! Destructor.
		inline ~FbxArrayBase_()
		{
			Clear();
		}

		inline int GetTypeSize() const { return mTypeSize.GetTypeSize(); }

		/** Get the number of elements in the array.
		  * \return The number of elements that conform the array's content.
		  */
		inline int GetCount() const { return GetArrayCount(); }

		/** All the elements of the array are removed and the memory of content is released.
		  * \remarks The destructor of the elements are not called.
		  */
		inline void Clear()
		{
			if (mBaseArray!=NULL)
			{
				FbxFree(mBaseArray);
				mBaseArray = NULL;
			}
		}

		/** All the elements of the array are removed, but the memory of content is not released.
		  * \remarks The destructor of the elements are not called.
		  */
		inline void Empty()
		{
		#ifdef _DEBUG
			memset(mBaseArray+ GetHeaderOffset(), 0, GetArrayCount()*GetTypeSize());
		#endif
			SetArrayCount(0);
		}

		/** Set the capacity of the allocated storage space for the elements of the array.
		  * \param pCapacity Minimum amount desired as capacity of allocated storage.
		  * \return The number of available slots in the array.
		  * \remarks If capacity is lower than arrayCount, arrayCount is lowered to capacity.
		  */
		inline int Reserve(int pCapacity)
		{
			FBX_ASSERT( pCapacity > 0 );
			if( pCapacity )
			{
				const FbxUInt lTempNewBlockCount = ( (FbxUInt) (pCapacity + FBXSDK_ARRAY_BLOCKSIZE - 1 ) / FBXSDK_ARRAY_BLOCKSIZE );
				const FbxUInt lNewBlockCount = (lTempNewBlockCount > 1 ? lTempNewBlockCount : 1);

				int lArrayCount = GetArrayCount();
				int lBlockCount = GetBlockCount();

				const FbxUInt lOldArraySize = lArrayCount*GetTypeSize();
				const FbxUInt lNewArraySize = lNewBlockCount*FBXSDK_ARRAY_BLOCKSIZE*GetTypeSize();

				if (lNewBlockCount != (FbxUInt) lBlockCount)
				{
					char* lBaseArray = (char*)FbxRealloc(mBaseArray, (size_t) lNewArraySize+ GetHeaderOffset()  );
					if (!lBaseArray)
						return GetBlockCount()*FBXSDK_ARRAY_BLOCKSIZE;
					mBaseArray = lBaseArray;
				}

				if( lNewBlockCount > (FbxUInt) lBlockCount ) {
					memset( ((char*)mBaseArray+ GetHeaderOffset() ) + lOldArraySize, 0, (size_t) (lNewArraySize-lOldArraySize) );
					SetArrayCount(lArrayCount);
				} 
				else if (pCapacity < lArrayCount)
				{
					memset( ((char*)mBaseArray+ GetHeaderOffset() ) + pCapacity*GetTypeSize(), 0, (size_t) (lNewArraySize-pCapacity*GetTypeSize()) );
					SetArrayCount(pCapacity);
				}

				SetBlockCount(lNewBlockCount);
			}

			return GetBlockCount()*FBXSDK_ARRAY_BLOCKSIZE;
		}

		/** Resizes the array to contain the specific elements.
		  * \param pCount New array size, expressed in elements.
		  * \remarks If the array is upsized, the memory allocated is set to 0 and
		  * no constructor is called. Thus, this function is not appropriate for
		  * types of elements requiring initialization.
		  */
		inline void SetCount(int pCount)
		{
		#ifdef _DEBUG
			if (pCount<0)
			{
				FBX_ASSERT_NOW ("ArrayUL : Item count can't be negative");
				return ;
			}
		#endif
			int lArrayCount = GetArrayCount();
			if (pCount > lArrayCount)
			{
				AddMultiple( pCount-lArrayCount);
			} 
			else
			{
				SetArrayCount(pCount);
			}
		}

		/** Resizes the array to contain the specific elements.
		  * \param pItemCount New array size, expressed in elements.
		  * \remarks If the array is upsized, the memory allocated is set to 0 and
		  * no constructor is called. Thus, this function is not appropriate for
		  * types of elements requiring initialization.
		  */
		inline void Resize(int pItemCount)
		{
			FBX_ASSERT( pItemCount >= 0 );

			const FbxUInt lTempNewBlockCount = ( (FbxUInt) (pItemCount + FBXSDK_ARRAY_BLOCKSIZE - 1 ) / FBXSDK_ARRAY_BLOCKSIZE );
			const FbxUInt lNewBlockCount = (lTempNewBlockCount > 1 ? lTempNewBlockCount : 1);

			int lArrayCount = GetArrayCount();
			int lBlockCount = GetBlockCount();

			const FbxUInt lOldArraySize = lArrayCount*GetTypeSize();
			const FbxUInt lNewArraySize = lNewBlockCount*FBXSDK_ARRAY_BLOCKSIZE*GetTypeSize();

			if (lNewBlockCount != (FbxUInt) lBlockCount)
			{
				char* lBaseArray = (char*)FbxRealloc(mBaseArray, (size_t) lNewArraySize+ GetHeaderOffset()  );
				if (!lBaseArray)
					return;
				mBaseArray = lBaseArray;
			}

			if( lNewBlockCount > (FbxUInt) lBlockCount )
			{
				memset( ((char*)mBaseArray+ GetHeaderOffset() ) + lOldArraySize, 0, (size_t) (lNewArraySize-lOldArraySize) );
			}
			else if (pItemCount < lArrayCount)
			{
				memset( ((char*)mBaseArray+ GetHeaderOffset() ) + pItemCount*GetTypeSize(), 0, (size_t) (lNewArraySize-pItemCount*GetTypeSize()) );
			}

			SetBlockCount(lNewBlockCount);
			SetArrayCount(pItemCount);
		}

		/** Append the specific number of elements to the end the array.
		  * \param pItemCount The number of elements appended in elements.
		  * \remarks The memory of new elements is set to 0 and
		  * no constructor is called. Thus, this function is not appropriate for
		  * types of elements requiring initialization.
		  */
		inline void AddMultiple(int pItemCount)
		{
			FBX_ASSERT( pItemCount > 0 );
			if( pItemCount )
			{
				int lArrayCount = GetArrayCount();
				int lBlockCount = GetBlockCount();
				const FbxUInt lTempNewBlockCount = ( (FbxUInt) (lArrayCount+pItemCount + FBXSDK_ARRAY_BLOCKSIZE - 1 ) / FBXSDK_ARRAY_BLOCKSIZE );
				const FbxUInt lNewBlockCount = (lTempNewBlockCount > 1 ? lTempNewBlockCount : 1);

				const FbxUInt lOldArraySize = lArrayCount*GetTypeSize();
				const FbxUInt lNewArraySize = lNewBlockCount*FBXSDK_ARRAY_BLOCKSIZE*GetTypeSize();

				FBX_ASSERT( lOldArraySize < lNewArraySize );

				if( lNewBlockCount > (FbxUInt) lBlockCount )
				{
					char* lBaseArray = (char*)FbxRealloc(mBaseArray, (size_t) lNewArraySize+ GetHeaderOffset()  );
					if (!lBaseArray)
						return;
					mBaseArray = lBaseArray;
					lBlockCount = lNewBlockCount;
				}

				memset( ((char*)mBaseArray+ GetHeaderOffset() ) + lOldArraySize, 0, (size_t) (lNewArraySize-lOldArraySize) );
				SetArrayCount ( lArrayCount + pItemCount );
				SetBlockCount (lBlockCount);
			}
		}

		/** Insert an item at the given position.
		  * \param pIndex Position where to insert the item.
		  * \param pItem  Pointer to the item to be inserted.
		  * \remarks if pIndex is greater than the number of items already in the
		  * array, the item will be appended at the end.
		  * \return The actual position where the item as been inserted.
		  */
		inline int InsertAt(int pIndex, void* pItem)
		{
			int lArrayCount = GetArrayCount();
			int lBlockCount = GetBlockCount();

			FBX_ASSERT( pIndex >= 0 );

			if (pIndex>lArrayCount) {
				pIndex = GetArrayCount();
			}

			if (lArrayCount>= lBlockCount*FBXSDK_ARRAY_BLOCKSIZE)
			{
				// must Alloc.Realloc some new space

				// double the number of blocks.
				lBlockCount = ( 0 == lBlockCount ) ? 1 : lBlockCount * 2;
				char* lBaseArray = (char*)FbxRealloc(mBaseArray, (size_t) (lBlockCount*FBXSDK_ARRAY_BLOCKSIZE*GetTypeSize()) + GetHeaderOffset() );
				if(!lBaseArray)
					return -1;
				mBaseArray = lBaseArray;
			}

			if (pIndex<lArrayCount)
			{
				// This is an insert
				memmove (&(mBaseArray[(pIndex+1)*GetTypeSize()+ GetHeaderOffset() ]), &(mBaseArray[(pIndex)*GetTypeSize()+ GetHeaderOffset()] ), GetTypeSize()*(lArrayCount-pIndex));
			}

			memmove (&(mBaseArray[(pIndex)*GetTypeSize()+ GetHeaderOffset() ]), pItem, GetTypeSize());

			SetArrayCount(lArrayCount+1);
			SetBlockCount(lBlockCount);

			return pIndex;
		}


		/** Get the item at the given position.
		  * \param pIndex The position of the item to access.
		  * \return Pointer to the item.
		  * \remarks This method assumes that the passed index is
		  * in the valid range of the array. No checks are made.
		  */
		inline void* GetAt(int pIndex) { return &(mBaseArray[(pIndex)*GetTypeSize()+ GetHeaderOffset()]); }

		/** Removes the item at the given position.
		  * \param pIndex The position of the item to remove.
		  * \remarks If the index is not valid, nothing is performed. Otherwise,
		  * the item is removed from the array and the items are shifted to fill the
		  * empty slot.
		  */
		inline void RemoveAt(int pIndex)
		{
		#if defined(_DEBUG) && !defined(FBXSDK_ENV_MAC)
			if (!ValidateIndex( pIndex ))
			{
				return;
			}
		#endif
			int lArrayCount = GetArrayCount();
			if (pIndex+1<lArrayCount)
			{
				memmove (&(mBaseArray[(pIndex)*GetTypeSize()+ GetHeaderOffset() ]), &(mBaseArray[(pIndex+1)*GetTypeSize()+ GetHeaderOffset() ]), GetTypeSize()*(lArrayCount-pIndex-1));
			}

			SetArrayCount( lArrayCount-1 );

		#ifdef _DEBUG
			memset( &(mBaseArray[(GetArrayCount())*GetTypeSize()+ GetHeaderOffset() ]),0,GetTypeSize());
		#endif
		}

		/** Check that the given position is inside the array boundaries.
		  * \param pIndex Index value to be validated.
		  * \return \c true if the index value is within the array boundaries. \c false
		  * otherwise.
		  */
		inline bool ValidateIndex( int pIndex ) const
		{
			int lArrayCount = GetArrayCount();
			if (pIndex>=0 && pIndex<lArrayCount)
			{
				return true;
			}
			else
			{
				FBX_ASSERT_NOW("Array : Index out of range");
				return false;
			}
		}

		inline const Header* GetHeader() const
		{
			return (const Header*)mBaseArray;
		}

		inline Header* GetHeader()
		{
			return (Header*)mBaseArray;
		}

		inline int GetHeaderOffset() const
		{
			return sizeof(Header);
		}

		inline int GetArrayCount() const
		{
			return GetHeader() ? GetHeader()->mArrayCount : 0;
		}

		inline void SetArrayCount(int pArrayCount)
		{
			if (GetHeader()) GetHeader()->mArrayCount = pArrayCount;
		}

		inline int GetBlockCount() const
		{
			return GetHeader() ? GetHeader()->mBlockCount : 0;
		}

		inline void SetBlockCount(int pArrayCount)
		{
			if (GetHeader()) GetHeader()->mBlockCount=pArrayCount;
		}

		char*		mBaseArray;
		TypeSize	mTypeSize;
	};

	// FbxArray<FbxArray<T> > is not supported.
	template <class Type> FBXSDK_INCOMPATIBLE_WITH_ARRAY_TEMPLATE(FbxArray<Type>);
#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_CORE_BASE_ARRAY_H_ */
