/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxmap.h
#ifndef _FBXSDK_CORE_BASE_MAP_H_
#define _FBXSDK_CORE_BASE_MAP_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/core/base/fbxcontainerallocators.h>
#include <fbxsdk/core/base/fbxpair.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

/* Examples of KEY_COMPARE_FUNCTOR class

   with KEY_TYPE = int
    class IntCompare {
        inline int operator()(int pKeyA, int pKeyB) const
        {
            return (pKeyA < pKeyB) ? -1 : ((pKeyB < pKeyA) ? 1 : 0);
        }
    };

   with KEY_TYPE = Class
    class ClassCompare {
        inline int operator()(const Class& pKeyA, const Class& pKeyB) const
        {
            return (pKeyA < pKeyB) ? -1 : ((pKeyB < pKeyA) ? 1 : 0);
        }
    };

   with KEY_TYPE = char*
    class StrCompare {
        inline int operator()(const char* pKeyA, const char* pKeyB) const
        {
            return strcmp(pKeyA, pKeyB);
        }
    };

*/

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <typename RecordType>    class FbxRedBack_ConstIteratorType;
template <typename RecordType>    class FbxRedBack_IteratorType;

template <typename RecordType>

class FbxRedBack_IteratorType
{
public:
    FbxRedBack_IteratorType() : mRecord(0) {}
    FbxRedBack_IteratorType(RecordType* pRecord) : mRecord(pRecord) {}
    FbxRedBack_IteratorType(const FbxRedBack_IteratorType<RecordType>& pV) : mRecord(pV.mRecord) {}

    // prefix ++
    FbxRedBack_IteratorType & operator++()
    {
        FBX_ASSERT( mRecord != NULL );
        mRecord = mRecord->Successor();
        return *this;
    }

    // postfix ++
    const FbxRedBack_IteratorType operator++(int)
    {
        FbxRedBack_IteratorType t(*this);
        operator++();
        return t;
    }

    // prefix --
    FbxRedBack_IteratorType & operator--()
    {
        FBX_ASSERT( mRecord );
        mRecord = mRecord->Predecessor();
        return *this;
    }

    // postfix --
    const FbxRedBack_IteratorType operator--(int)
    {
        FbxRedBack_IteratorType t(*this);
        operator--();
        return t;
    }

    const RecordType& operator*() const
    {
        FBX_ASSERT( mRecord );

        return *mRecord;
    }

    RecordType& operator*()
    {
        FBX_ASSERT( mRecord );

        return *mRecord;
    }

    const RecordType* operator->() const
    {
        FBX_ASSERT( mRecord );

        return mRecord;
    }

    RecordType* operator->()
    {
        FBX_ASSERT( mRecord );

        return mRecord;
    }

    inline bool operator==(const FbxRedBack_IteratorType& pOther) const
    {
        return mRecord == pOther.mRecord;
    }

    inline bool operator !=(const FbxRedBack_IteratorType& pOther) const
    {
        return mRecord != pOther.mRecord;
    }

protected:
    friend class FbxRedBack_ConstIteratorType<RecordType>;

    RecordType* mRecord;
};

template <typename RecordType>
class FbxRedBack_ConstIteratorType
{
public:
    FbxRedBack_ConstIteratorType() : mRecord(0) {}
    FbxRedBack_ConstIteratorType(const RecordType* pRecord) : mRecord(pRecord) {}
    FbxRedBack_ConstIteratorType(const FbxRedBack_IteratorType<RecordType>& pV) : mRecord(pV.mRecord) {}
    FbxRedBack_ConstIteratorType(const FbxRedBack_ConstIteratorType<RecordType>& pV) : mRecord(pV.mRecord) {}

    // prefix ++
    FbxRedBack_ConstIteratorType & operator++()
    {
        FBX_ASSERT( mRecord != NULL );
        mRecord = mRecord->Successor();
        return *this;
    }

    // postfix ++
    const FbxRedBack_ConstIteratorType operator++(int)
    {
        FbxRedBack_ConstIteratorType t(*this);
        operator++();
        return t;
    }

    // prefix --
    FbxRedBack_ConstIteratorType & operator--()
    {
        FBX_ASSERT( mRecord );
        mRecord = mRecord->Predecessor();
        return *this;
    }

    // postfix --
    const FbxRedBack_ConstIteratorType operator--(int)
    {
        FbxRedBack_ConstIteratorType t(*this);
        operator--();
        return t;
    }

    const RecordType& operator*() const
    {
        FBX_ASSERT( mRecord );

        return *mRecord;
    }

    const RecordType& operator*()
    {
        FBX_ASSERT( mRecord );

        return *mRecord;
    }

    const RecordType* operator->() const
    {
        FBX_ASSERT( mRecord );

        return mRecord;
    }

    const RecordType* operator->()
    {
        FBX_ASSERT( mRecord );

        return mRecord;
    }

    inline bool operator==(const FbxRedBack_ConstIteratorType& pOther) const
    {
        return mRecord == pOther.mRecord;
    }

    inline bool operator !=(const FbxRedBack_ConstIteratorType& pOther) const
    {
        return mRecord != pOther.mRecord;
    }

protected:
    friend class FbxRedBack_IteratorType<RecordType>;

    const RecordType* mRecord;
};

/**Implements an efficient ordered data storage.
  */
template <typename DATA_TYPE,
          typename KEY_COMPARE_FUNCTOR,
          typename ALLOCATOR>
class  FbxRedBlackTree
{
public:
    typedef DATA_TYPE DataType;
    typedef typename DATA_TYPE::KeyType         KeyType;
    typedef typename DATA_TYPE::ConstKeyType    ConstKeyType;
    typedef typename DATA_TYPE::ValueType       ValueType;
    typedef typename DATA_TYPE::ConstValueType  ConstValueType;
    typedef ALLOCATOR AllocatorType;

    /**
       This class represents a node in the tree. It contains the key,
       the value, and internal tree management data.
    */
    class RecordType
    {
    public:
        inline ConstKeyType& GetKey() const { return mData.GetKey(); }
        inline ConstValueType& GetValue() const { return mData.GetValue(); }
        inline ValueType& GetValue() { return mData.GetValue(); }

        inline const RecordType* Minimum() const
        {
            const RecordType* lParent = 0;
            const RecordType* lNode = this;
            while (lNode != 0)
            {
                lParent = lNode;
                lNode = lNode->mLeftChild;
            }

            return lParent;
        }

        inline RecordType* Minimum()
        {
            RecordType* lParent = 0;
            RecordType* lNode = this;
            while (lNode != 0)
            {
                lParent = lNode;
                lNode = lNode->mLeftChild;
            }

            return lParent;
        }

        inline const RecordType* Maximum() const
        {
            const RecordType* lParent = 0;
            const RecordType* lNode = this;
            while (lNode != 0)
            {
                lParent = lNode;
                lNode = lNode->mRightChild;
            }

            return lParent;
        }

        inline RecordType* Maximum()
        {
            RecordType* lParent = 0;
            RecordType* lNode = this;
            while (lNode != 0)
            {
                lParent = lNode;
                lNode = lNode->mRightChild;
            }

            return lParent;
        }

        inline const RecordType* Predecessor() const
        {
            if (mLeftChild)
            {
                return mLeftChild->Maximum();
            }
            else
            {
                const RecordType* lParent = mParent;
                const RecordType* lNode = this;

                while (lParent && lParent->mLefttChild == lNode)
                {
                    lNode = lParent;
                    lParent = lParent->mParent;
                }

                return lParent;
            }
        }

        inline RecordType* Predecessor()
        {
            if (mLeftChild)
            {
                return mLeftChild->Maximum();
            }
            else
            {
                RecordType* lParent = mParent;
                RecordType* lNode = this;

                while (lParent && lParent->mLeftChild == lNode)
                {
                    lNode = lParent;
                    lParent = lParent->mParent;
                }

                return lParent;
            }
        }

        inline const RecordType* Successor() const
        {
            if (mRightChild)
            {
                return mRightChild->Minimum();
            }
            else
            {
                const RecordType* lParent = mParent;
                const RecordType* lNode = this;

                while (lParent && lParent->mRightChild == lNode)
                {
                    lNode = lParent;
                    lParent = lParent->mParent;
                }

                return lParent;
            }
        }

        inline RecordType* Successor()
        {
            if (mRightChild)
            {
                return mRightChild->Minimum();
            }
            else
            {
                RecordType* lParent = mParent;
                RecordType* lNode = this;

                while (lParent && lParent->mRightChild == lNode)
                {
                    lNode = lParent;
                    lParent = lParent->mParent;
                }

                return lParent;
            }
        }

        inline const int GetBlackDepth() { return mBlackDepth; }

    private:
        enum ETreeType {eRed, eBlack};

        inline RecordType(const DataType& pData)
            : mData(pData)
            , mParent(0)
            , mLeftChild(0)
            , mRightChild(0)
            , mColor(eRed)
            , mBlackDepth(0)
        {
        }

        inline RecordType(const RecordType& pRecordType)
            : mData(pRecordType.mData)
            , mParent(0)
            , mLeftChild(0)
            , mRightChild(0)
            , mColor(pRecordType.mColor)
            , mBlackDepth(pRecordType.mBlackDepth)
        {
        }

        DataType mData;

        friend class FbxRedBlackTree;

        RecordType* mParent;
        RecordType* mLeftChild;
        RecordType* mRightChild;
        unsigned int mColor:2;
        unsigned int mBlackDepth:30;
    };

public:
    typedef FbxRedBack_ConstIteratorType<RecordType>  ConstIteratorType;
    typedef FbxRedBack_IteratorType<RecordType>       IteratorType;

public:
    inline FbxRedBlackTree() : mRoot(0), mSize(0), mAllocator(sizeof(RecordType)) {}
    inline FbxRedBlackTree(const FbxRedBlackTree& pTree) : mRoot(0), mSize(0), mAllocator(sizeof(RecordType)) { operator=(pTree); }
    inline ~FbxRedBlackTree() { Clear(); }

    /** Deep copy pTree in this. 
      * \param pTree
      */
    inline FbxRedBlackTree& operator=(const FbxRedBlackTree& pTree)
    {
        if( this != &pTree )
        {
            Clear();

            mAllocator = pTree.mAllocator;

            if( pTree.mRoot )
            {
                void* lBuffer = mAllocator.AllocateRecords();
                mRoot = new(lBuffer) RecordType(*(pTree.mRoot));
                mRoot->mLeftChild = DuplicateSubTree(pTree.mRoot->mLeftChild);
                mRoot->mRightChild = DuplicateSubTree(pTree.mRoot->mRightChild);

                if (mRoot->mLeftChild)
                {
                    mRoot->mLeftChild->mParent = mRoot;
                }

                if (mRoot->mRightChild)
                {
                    mRoot->mRightChild->mParent = mRoot;
                }
            }
            else
            {
                FBX_ASSERT( pTree.mSize == 0 );
                FBX_ASSERT( mRoot == 0 );
            }

            mSize = pTree.mSize;
        }

        return *this;
    }

    inline bool operator==(const FbxRedBlackTree& pTree) const
    {
        // Check a few quick shortcuts
        if( this == &pTree )
            return true;

        if( GetSize() != pTree.GetSize() )
            return false;

        // Iterator through all nodes; if we reach the end of both iterators at the same
        // time then we have two iterators that match.
        ConstIteratorType End;
        ConstIteratorType Iter1(Minimum());
        ConstIteratorType Iter2(pTree.Minimum());

        while( (Iter1 != End) && (Iter2 != End) &&
               (Iter1->GetKey() == Iter2->GetKey()) &&
               (Iter1->GetValue() == Iter2->GetValue()) )
        {
            ++Iter1;
            ++Iter2;
        }

        return Iter1 == End && Iter2 == End;
    }

    /** Ask ALLOCATOR to reserve space to hold pRecordCount elements. 
      * \param pRecordCount
      */
    inline void Reserve(unsigned int pRecordCount)
    {
        mAllocator.Reserve(pRecordCount);
    }

    /** Get the number of elements in the tree. Takes O(1) time.
      * \return The number of elements in the tree.
      */
    inline int GetSize() const { return mSize; }

    inline bool Empty() const { return mSize == 0; }

    /** Insert a new element in the tree. Takes O(log n) time.
      * \param pData The element to insert.
      * \return If pData.GetKey() is already present in the tree, returns the
      *         existing record and false; else returns the new record and true.
      */
    inline FbxPair<RecordType*, bool> Insert(const DataType& pData)
    {
        KEY_COMPARE_FUNCTOR lCompareKeys;
        bool lResult = false;
        RecordType* lParent = 0;
        RecordType* lNode = mRoot;
        while (lNode != 0)
        {
            const KeyType& lNodeKey = lNode->GetKey();
            const KeyType& lDataKey = pData.GetKey();

            if (lCompareKeys(lNodeKey, lDataKey) < 0)
            {
                lParent = lNode;
                lNode = lNode->mRightChild;
            }
            else if (lCompareKeys(lNodeKey, lDataKey) > 0)
            {
                lParent = lNode;
                lNode = lNode->mLeftChild;
            }
            else
            {
                break;
            }
        }

        if (lNode == 0)
        {
            void* lBuffer = mAllocator.AllocateRecords();
            lNode = new(lBuffer) RecordType(pData);
            mSize++;

			FBX_ASSERT(lNode == lBuffer);

            if (lParent)
            {
                if (lCompareKeys(lParent->GetKey(), pData.GetKey()) < 0)
                {
                    FBX_ASSERT(lParent->mRightChild == 0);
                    lParent->mRightChild = lNode;
                    lNode->mParent = lParent;
                }
                else
                {
                    FBX_ASSERT(lParent->mLeftChild == 0);
                    lParent->mLeftChild = lNode;
                    lNode->mParent = lParent;
                }
            }
            else
            {
                mRoot = lNode;
            }

            // Fix red black tree property
            FixNodesAfterInsertion(lNode);

            lResult = true;
        }

        return FbxPair<RecordType*, bool>(lNode, lResult);
    }

    /** Remove an element identified by a key from the tree. Takes O(log n) time.
      * \param pKey The key identifying the element to remove.
      */
    inline bool Remove(const KeyType& pKey)
    {
        KEY_COMPARE_FUNCTOR lCompareKeys;
        bool lResult = false;
        RecordType* lNode = mRoot;
        while (lNode != 0)
        {
            if (lCompareKeys(lNode->GetKey(), pKey) < 0)
            {
                lNode = lNode->mRightChild;
            }
            else if (lCompareKeys(lNode->GetKey(), pKey) > 0)
            {
                lNode = lNode->mLeftChild;
            }
            else
            {
                break;
            }
        }

        if (lNode)
        {
            RemoveNode(lNode);
            mSize--;
            lNode->~RecordType();
            mAllocator.FreeMemory(lNode);

            lResult = true;
        }

        return lResult;
    }

    /** Remove all elements from the tree. Takes O(n) time. Recursive.
      */
    inline void Clear()
    {
        if (mRoot)
        {
            ClearSubTree(mRoot->mLeftChild);
            ClearSubTree(mRoot->mRightChild);
            mRoot->~RecordType();
            mAllocator.FreeMemory(mRoot);
            mRoot = 0;
            mSize = 0;
        }
    }

    /** Find the smallest element in the tree.
      * Takes O(log n) time.
      */
    inline const RecordType* Minimum() const
    {
        if (0 != mRoot)
        {
            return mRoot->Minimum();
        }
        else
        {
            return 0;
        }
    }

    /** Find the smallest element in the tree.
      * Takes O(log n) time.
      */
    inline RecordType* Minimum()
    {
        if (0 != mRoot)
        {
            return mRoot->Minimum();
        }
        else
        {
            return 0;
        }
    }

    /** Find the largest element in the tree.
      * Takes O(log n) time.
      */
    inline const RecordType* Maximum() const
    {
        if (0 != mRoot)
        {
            return mRoot->Maximum();
        }
        else
        {
            return 0;
        }
    }

    /** Find the largest element in the tree.
      * Takes O(log n) time.
      */
    inline RecordType* Maximum()
    {
        if (0 != mRoot)
        {
            return mRoot->Maximum();
        }
        else
        {
            return 0;
        }
    }

    /** Find the key-value pair with key pKey.
      * Takes O(log n) time.
      * \param pKey The key to look for.
      */
    inline const RecordType* Find(const KeyType& pKey) const
    {
        KEY_COMPARE_FUNCTOR lCompareKeys;
        const RecordType* lNode = mRoot;
        while (lNode != 0)
        {
            if (lCompareKeys(lNode->GetKey(), pKey) < 0)
            {
                lNode = lNode->mRightChild;
            }
            else if (lCompareKeys(lNode->GetKey(), pKey) > 0)
            {
                lNode = lNode->mLeftChild;
            }
            else
            {
                break;
            }
        }

        return lNode;
    }

    /** Find the key-value pair with key pKey.
      * Takes O(log n) time.
      * \param pKey The key to look for.
      */
    inline RecordType* Find(const KeyType& pKey)
    {
        KEY_COMPARE_FUNCTOR lCompareKeys;
        RecordType* lNode = mRoot;
        while (lNode != 0)
        {
            if (lCompareKeys(lNode->GetKey(), pKey) < 0)
            {
                lNode = lNode->mRightChild;
            }
            else if (lCompareKeys(lNode->GetKey(), pKey) > 0)
            {
                lNode = lNode->mLeftChild;
            }
            else
            {
                break;
            }
        }

        return lNode;
    }

    /** Find the key-value pair with the smallest key greater than pKey.
      * Takes O(log n) time.
      * \param pKey The key to look for.
      */
    inline const RecordType* UpperBound(const KeyType& pKey) const
    {
        KEY_COMPARE_FUNCTOR lCompareKeys;
        const RecordType* lNode = mRoot;
        const RecordType* lCandidate = 0;
        while (lNode != 0)
        {
            if (lCompareKeys(lNode->GetKey(), pKey) <= 0)
            {
                lNode = lNode->mRightChild;
            }
            else if (lCompareKeys(lNode->GetKey(), pKey) > 0)
            {
                lCandidate = lNode;
                lNode = lNode->mLeftChild;
            }
        }
        
        return lCandidate;
    }

    /** Find the key-value pair with the smallest key greater than pKey.
      * Takes O(log n) time.
      * \param pKey The key to look for.
      */
    inline RecordType* UpperBound(const KeyType& pKey)
    {
        KEY_COMPARE_FUNCTOR lCompareKeys;
        RecordType* lNode = mRoot;
        RecordType* lCandidate = 0;
        while (lNode != 0)
        {
            if (lCompareKeys(lNode->GetKey(), pKey) <= 0)
            {
                lNode = lNode->mRightChild;
            }
            else if (lCompareKeys(lNode->GetKey(), pKey) > 0)
            {
                lCandidate = lNode;
                lNode = lNode->mLeftChild;
            }
        }
        
        return lCandidate;
    }

protected:
    RecordType* mRoot;
    int mSize;

    AllocatorType mAllocator;

    /** Sanity check on the tree. Check if all red-black tree
      * properties hold. Also check if all key-values pairs are ordered
      * properly.
      */
    inline void IsSane()
    {
        FBX_ASSERT((mRoot == 0) || (mRoot->mColor == RecordType::eBlack));
        FBX_ASSERT(((mRoot == 0) && (mSize == 0)) || (mRoot != 0) && (mSize != 0));
        IsSubTreeSane(mRoot);

        ComputeBlackDepth(mRoot, 0);

        RecordType* lNode = mRoot;
        unsigned int lLeafBlackDepth = 0;
        while (lNode)
        {
            if (lNode->mLeftChild == 0)
            {
                lLeafBlackDepth = lNode->mBlackDepth + ((lNode->mColor == RecordType::eBlack) ? 1 : 0);
            }

            lNode = lNode->mLeftChild;
        }

        CheckLeavesBlackDepth(mRoot, lLeafBlackDepth);
    }

    inline void IsSubTreeSane(const RecordType* pNode) const
    {
        KEY_COMPARE_FUNCTOR lCompareKeys;

        if (pNode)
        {
            FBX_ASSERT(pNode != pNode->mParent);
            FBX_ASSERT(pNode != pNode->mLeftChild);
            FBX_ASSERT(pNode != pNode->mRightChild);

            // Check for two consecutive red nodes
            FBX_ASSERT((pNode->mColor == RecordType::eBlack) ||
                     (pNode->mLeftChild == NULL) ||
                     (pNode->mLeftChild->mColor == RecordType::eBlack));

            FBX_ASSERT((pNode->mColor == RecordType::eBlack) ||
                     (pNode->mRightChild == NULL) ||
                     (pNode->mRightChild->mColor == RecordType::eBlack));

            // Check key ordering
            FBX_ASSERT((pNode->mLeftChild == 0 ||
                      lCompareKeys(pNode->GetKey(), pNode->mLeftChild->GetKey()) > 0));

            FBX_ASSERT((pNode->mRightChild == 0 ||
                      lCompareKeys(pNode->GetKey(), pNode->mRightChild->GetKey()) < 0));

            IsSubTreeSane(pNode->mLeftChild);
            IsSubTreeSane(pNode->mRightChild);
        }
    }

    inline void ComputeBlackDepth(RecordType* pNode, unsigned int pDepth)
    {
        if (pNode)
        {
            pNode->mBlackDepth = pDepth;
            if (pNode->mColor == RecordType::eBlack)
            {
                pDepth++;
            }

            ComputeBlackDepth(pNode->mLeftChild, pDepth);
            ComputeBlackDepth(pNode->mRightChild, pDepth);
        }
    }

    inline void CheckLeavesBlackDepth(RecordType* pNode, unsigned int pBlackDepth)
    {
        if (pNode)
        {
            if ((pNode->mLeftChild == 0) || pNode->mRightChild == 0)
            {
                int lBlackDepth = pNode->mBlackDepth + ((pNode->mColor == RecordType::eBlack) ? 1 : 0);
                FBX_ASSERT(lBlackDepth == pBlackDepth);
            }

            CheckLeavesBlackDepth(pNode->mLeftChild, pBlackDepth);
            CheckLeavesBlackDepth(pNode->mRightChild, pBlackDepth);
        }
    }

    inline RecordType* DuplicateSubTree(const RecordType* pNode)
    {
        RecordType* lNewSubTree = 0;

        if (pNode)
        {
            void* lBuffer = mAllocator.AllocateRecords();
            lNewSubTree = new(lBuffer) RecordType(*pNode);
            lNewSubTree->mLeftChild = DuplicateSubTree(pNode->mLeftChild);
            lNewSubTree->mRightChild = DuplicateSubTree(pNode->mRightChild);

            if (lNewSubTree->mLeftChild)
            {
                lNewSubTree->mLeftChild->mParent = lNewSubTree;
            }

            if (lNewSubTree->mRightChild)
            {
                lNewSubTree->mRightChild->mParent = lNewSubTree;
            }
        }

        return lNewSubTree;
    }

    inline void FixNodesAfterInsertion(RecordType* pNode)
    {
        RecordType* lNode = pNode;
        bool lDone = false;

        while (!lDone)
        {
            lDone = true;

            if (lNode->mParent == 0)
            {
                lNode->mColor = RecordType::eBlack;
            }
            else if (lNode->mParent->mColor == RecordType::eRed)
            {
                RecordType* lUncle = 0;
                if (lNode->mParent == lNode->mParent->mParent->mLeftChild)
                {
                    lUncle = lNode->mParent->mParent->mRightChild;
                }
                else if (lNode->mParent == lNode->mParent->mParent->mRightChild)
                {
                    lUncle = lNode->mParent->mParent->mLeftChild;
                }

                // since lNode->mParent is red, lNode->mParent->mParent exists

                if (lUncle && lUncle->mColor == RecordType::eRed)
                {
                    lNode->mParent->mColor = RecordType::eBlack;
                    lUncle->mColor = RecordType::eBlack;
                    lNode->mParent->mParent->mColor = RecordType::eRed;
                    lNode = lNode->mParent->mParent;

                    lDone = false;
                }
                else
                {
                    if ((lNode == lNode->mParent->mRightChild) &&
                        (lNode->mParent == lNode->mParent->mParent->mLeftChild))
                    {
                        LeftRotate(lNode->mParent);
                        lNode = lNode->mLeftChild;
                    }
                    else if ((lNode == lNode->mParent->mLeftChild) &&
                            (lNode->mParent == lNode->mParent->mParent->mRightChild))
                    {
                        RightRotate(lNode->mParent);
                        lNode = lNode->mRightChild;
                    }

                    lNode->mParent->mColor = RecordType::eBlack;
                    lNode->mParent->mParent->mColor = RecordType::eRed;
                    if ((lNode == lNode->mParent->mLeftChild) &&
                        (lNode->mParent == lNode->mParent->mParent->mLeftChild))
                    {
                        RightRotate(lNode->mParent->mParent);
                    }
                    else
                    {
                        LeftRotate(lNode->mParent->mParent);
                    }
                }
            }
        }

        mRoot->mColor = RecordType::eBlack;
    }

    inline void LeftRotate(RecordType* pNode)
    {
        RecordType* lNode = pNode->mRightChild;

#ifdef _DEBUG
        RecordType* A = pNode->mLeftChild;
        RecordType* B = lNode->mLeftChild;
        RecordType* C = lNode->mRightChild;
        RecordType* Z = pNode->mParent;
#endif

        pNode->mRightChild = lNode->mLeftChild;
        if (pNode->mRightChild)
        {
            pNode->mRightChild->mParent = pNode;
        }

        lNode->mParent = pNode->mParent;
        if (pNode->mParent == 0)
        {
            FBX_ASSERT(mRoot == pNode);
            mRoot = lNode;
        }
        else if (pNode == pNode->mParent->mLeftChild)
        {
            pNode->mParent->mLeftChild = lNode;
        }
        else
        {
            pNode->mParent->mRightChild = lNode;
        }
        pNode->mParent = lNode;
        lNode->mLeftChild = pNode;

        FBX_ASSERT(pNode->mLeftChild == A);
        FBX_ASSERT(pNode->mRightChild == B);
        FBX_ASSERT(pNode->mParent == lNode);

        FBX_ASSERT(lNode->mLeftChild == pNode);
        FBX_ASSERT(lNode->mRightChild == C);
        FBX_ASSERT(lNode->mParent == Z);

        FBX_ASSERT(A == 0 || A->mParent == pNode);
        FBX_ASSERT(B == 0 || B->mParent == pNode);
        FBX_ASSERT(C == 0 || C->mParent == lNode);
        FBX_ASSERT(Z == 0 || Z->mLeftChild == lNode || Z->mRightChild == lNode);
    }


    inline void RightRotate(RecordType* pNode)
    {
        RecordType* lNode = pNode->mLeftChild;

#ifdef _DEBUG
        RecordType* A = lNode->mLeftChild;
        RecordType* B = lNode->mRightChild;
        RecordType* C = pNode->mRightChild;
        RecordType* Z = pNode->mParent;
#endif

        pNode->mLeftChild = lNode->mRightChild;
        if (pNode->mLeftChild)
        {
            pNode->mLeftChild->mParent = pNode;
        }

        lNode->mParent = pNode->mParent;
        if (pNode->mParent == 0)
        {
            FBX_ASSERT(mRoot == pNode);
            mRoot = lNode;
        }
        else if (pNode == pNode->mParent->mRightChild)
        {
            pNode->mParent->mRightChild = lNode;
        }
        else
        {
            pNode->mParent->mLeftChild = lNode;
        }
        pNode->mParent = lNode;
        lNode->mRightChild = pNode;

        FBX_ASSERT(lNode->mLeftChild == A);
        FBX_ASSERT(lNode->mRightChild == pNode);
        FBX_ASSERT(lNode->mParent == Z);

        FBX_ASSERT(pNode->mLeftChild == B);
        FBX_ASSERT(pNode->mRightChild == C);
        FBX_ASSERT(pNode->mParent == lNode);

        FBX_ASSERT(A == 0 || A->mParent == lNode);
        FBX_ASSERT(B == 0 || B->mParent == pNode);
        FBX_ASSERT(C == 0 || C->mParent == pNode);
        FBX_ASSERT(Z == 0 || Z->mLeftChild == lNode || Z->mRightChild == lNode);
    }


    inline void RemoveNode(RecordType* pNode)
    {
        if (pNode->mLeftChild == 0)
        {
            if (pNode->mRightChild == 0)
            {
                if (pNode->mParent)
                {
                    if (pNode->mParent->mLeftChild == pNode)
                    {
                        pNode->mParent->mLeftChild = 0;
                    }
                    else if (pNode->mParent->mRightChild == pNode)
                    {
                        pNode->mParent->mRightChild = 0;
                    }
                    else
                    {
                        FBX_ASSERT_NOW("Node not found in FbxRedBlackTree");
                    }
                }
                else
                {
                    FBX_ASSERT(mRoot == pNode);
                    mRoot = 0;
                }

                if (pNode->mColor == RecordType::eBlack)
                {
                    FixNodesAfterRemoval(pNode->mParent, 0);
                }
            }
            else
            {
                if (pNode->mParent)
                {
                    if (pNode->mParent->mLeftChild == pNode)
                    {
                        pNode->mParent->mLeftChild = pNode->mRightChild;
                        pNode->mRightChild->mParent = pNode->mParent;
                    }
                    else if (pNode->mParent->mRightChild == pNode)
                    {
                        pNode->mParent->mRightChild = pNode->mRightChild;
                        pNode->mRightChild->mParent = pNode->mParent;
                    }
                    else
                    {
                        FBX_ASSERT_NOW("Node not found in FbxRedBlackTree");
                    }
                }
                else
                {
                    FBX_ASSERT(mRoot == pNode);
                    mRoot = pNode->mRightChild;
                    pNode->mRightChild->mParent = 0;
                }

                if (pNode->mColor == RecordType::eBlack)
                {
                    FixNodesAfterRemoval(pNode->mRightChild->mParent, pNode->mRightChild);
                }
            }
        }
        else
        {
            if (pNode->mRightChild == 0)
            {
                if (pNode->mParent)
                {
                    if (pNode->mParent->mLeftChild == pNode)
                    {
                        pNode->mParent->mLeftChild = pNode->mLeftChild;
                        pNode->mLeftChild->mParent = pNode->mParent;
                    }
                    else if (pNode->mParent->mRightChild == pNode)
                    {
                        pNode->mParent->mRightChild = pNode->mLeftChild;
                        pNode->mLeftChild->mParent = pNode->mParent;
                    }
                    else
                    {
                        FBX_ASSERT_NOW("Node not found in FbxRedBlackTree");
                    }
                }
                else
                {
                    FBX_ASSERT(mRoot == pNode);
                    mRoot = pNode->mLeftChild;
                    pNode->mLeftChild->mParent = 0;
                }

                if (pNode->mColor == RecordType::eBlack)
                {
                    FixNodesAfterRemoval(pNode->mLeftChild->mParent, pNode->mLeftChild);
                }
            }
            else
            {
                RecordType* lMinRightNode = pNode->mRightChild->Minimum();
                RemoveNode(lMinRightNode);

                lMinRightNode->mColor = pNode->mColor;
                ReplaceNode(pNode, lMinRightNode);
            }
        }

        pNode->mParent = 0;
        pNode->mLeftChild = 0;
        pNode->mRightChild = 0;
    }


    inline void ReplaceNode(RecordType* pNodeToReplace, RecordType* pReplacement)
    {
        pReplacement->mParent = pNodeToReplace->mParent;
        if (pNodeToReplace->mParent)
        {
            if (pNodeToReplace->mParent->mLeftChild == pNodeToReplace)
            {
                pNodeToReplace->mParent->mLeftChild = pReplacement;
            }
            else if (pNodeToReplace->mParent->mRightChild == pNodeToReplace)
            {
                pNodeToReplace->mParent->mRightChild = pReplacement;
            }
        }
        else
        {
            FBX_ASSERT(mRoot == pNodeToReplace);
            mRoot = pReplacement;
        }

        pReplacement->mLeftChild = pNodeToReplace->mLeftChild;
        if (pReplacement->mLeftChild)
        {
            pReplacement->mLeftChild->mParent = pReplacement;
        }

        pReplacement->mRightChild = pNodeToReplace->mRightChild;
        if (pReplacement->mRightChild)
        {
            pReplacement->mRightChild->mParent = pReplacement;
        }
    }

    inline RecordType* Sibling(const RecordType* pParent, const RecordType* pNode) const
    {
        if (pParent)
        {
            if (pParent->mLeftChild == pNode)
            {
                return pParent->mRightChild;
            }
            else if (pParent->mRightChild == pNode)
            {
                return pParent->mLeftChild;
            }
        }

        return 0;
    }

    inline bool IsBlack(const RecordType* pNode)
    {
        return ((pNode == 0) || (pNode->mColor == RecordType::eBlack));
    }

    inline void FixNodesAfterRemoval(RecordType* pParent, RecordType* pNode)
    {
        RecordType* lParent = pParent;
        RecordType* lNode = pNode;
        bool lDone = false;

        while (!lDone)
        {
            lDone = true;

            if (!IsBlack(lNode))
            {
                lNode->mColor = RecordType::eBlack;
            }
            else if (lParent != NULL)
            {
                RecordType* lSibling = Sibling(lParent, lNode);

                if (!IsBlack(lSibling))
                {
                    lParent->mColor = RecordType::eRed;
                    lSibling->mColor = RecordType::eBlack;
                    if (lNode == lParent->mLeftChild)
                    {
                        LeftRotate(lParent);
                    }
                    else
                    {
                        RightRotate(lParent);
                    }

                    // update sibling: it may have change after rotation
                    // parent was not affected by this rotation
                    lSibling = Sibling(lParent, lNode);
                }

                /* check this for null sibling */
                if (lSibling &&
                    IsBlack(lParent) &&
                    IsBlack(lSibling) &&
                    IsBlack(lSibling->mLeftChild) &&
                    IsBlack(lSibling->mRightChild))
                {
                    lSibling->mColor = RecordType::eRed;
                    lNode = lParent;
                    lParent = lParent->mParent;
                    lDone = false;
                }
                else
                {
                    if (!IsBlack(lParent) &&
                        IsBlack(lSibling) &&
                        ((lSibling == 0) || IsBlack(lSibling->mLeftChild)) &&
                        ((lSibling == 0) || IsBlack(lSibling->mRightChild)))
                    {
                        if (lSibling)
                        {
                            lSibling->mColor = RecordType::eRed;
                        }
                        lParent->mColor = RecordType::eBlack;
                    }
                    else // lSibling != 0
                    {
                        if ((lNode == lParent->mLeftChild) &&
                            IsBlack(lSibling) &&
                            !IsBlack(lSibling->mLeftChild) &&
                            IsBlack(lSibling->mRightChild))
                        {
                            lSibling->mColor = RecordType::eRed;
                            lSibling->mLeftChild->mColor = RecordType::eBlack;
                            RightRotate(lSibling);
                        }
                        else if ((lNode == lParent->mRightChild) &&
                                 IsBlack(lSibling) &&
                                 IsBlack(lSibling->mLeftChild) &&
                                 !IsBlack(lSibling->mRightChild))
                        {
                            lSibling->mColor = RecordType::eRed;
                            lSibling->mRightChild->mColor = RecordType::eBlack;
                            LeftRotate(lSibling);
                        }

                        // update sibling: it may have change after rotation
                        lSibling = Sibling(lParent, lNode);
                        FBX_ASSERT(lSibling != 0); // lSibling is now
                                                 // the former red
                                                 // child of the
                                                 // former sibling

                        lSibling->mColor = lParent->mColor;
                        lParent->mColor = RecordType::eBlack;
                        if (lNode == lParent->mLeftChild)
                        {
                            if (lSibling->mRightChild)
                            {
                                lSibling->mRightChild->mColor = RecordType::eBlack;
                            }
                            LeftRotate(lParent);
                        }
                        else
                        {
                            if (lSibling->mLeftChild)
                            {
                                lSibling->mLeftChild->mColor = RecordType::eBlack;
                            }
                            RightRotate(lParent);
                        }
                    }
                }
            }
        }

        if (mRoot)
        {
            mRoot->mColor = RecordType::eBlack;
        }
    }


    inline void ClearSubTree(RecordType* pNode)
    {
        if (pNode)
        {
            ClearSubTree(pNode->mLeftChild);
            ClearSubTree(pNode->mRightChild);
            pNode->~RecordType();
            mAllocator.FreeMemory(pNode);
        }
    }

    inline int GetSubTreeSize(RecordType* pNode) const
    {
        if (pNode)
        {
            return GetSubTreeSize(pNode->mLeftChild) + GetSubTreeSize(pNode->mRightChild) + 1;
        }
        else
        {
            return 0;
        }
    }

};

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

/** Default comparator for FbxMap and FbxSet2, which assumes operator < is defined.
*/
template <typename T>
struct FbxLessCompare
{
    inline int operator()(const T& left, const T& right) const
    {
        return (left < right) ? -1 : ((right < left) ? 1 : 0);
    }
};

/** This class implements an efficient map based on key comparison, which stores key-value pairs.
  * It executes insertion, deletion and query operations in O(log(n)) time.
  */
template <typename KEY_TYPE,
          typename VALUE_TYPE,
          typename KEY_COMPARE_FUNCTOR = FbxLessCompare<KEY_TYPE>,
          typename ALLOCATOR = FbxBaseAllocator>
class FbxMap
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS

protected:
    //! This class defines the key-value pairs used in FbxMap.
    class FbxKeyValuePair : private FbxPair<const KEY_TYPE, VALUE_TYPE>
    {
    public:
        FbxKeyValuePair(const KEY_TYPE& pFirst, const VALUE_TYPE& pSecond)
            : FbxPair<const KEY_TYPE, VALUE_TYPE>(pFirst, pSecond) {}

        // Key is always const.
        typedef const KEY_TYPE      KeyType;
        typedef const KEY_TYPE      ConstKeyType;
        typedef VALUE_TYPE          ValueType;
        typedef const VALUE_TYPE    ConstValueType;

        ConstKeyType & GetKey() const       { return this->mFirst; }
        KeyType & GetKey()                  { return this->mFirst; }

        ConstValueType& GetValue() const    { return this->mSecond; }
        ValueType& GetValue()               { return this->mSecond; }
    };

    typedef FbxRedBlackTree<FbxKeyValuePair, KEY_COMPARE_FUNCTOR, ALLOCATOR> StorageType;
    StorageType mTree;

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

public:
    typedef VALUE_TYPE ValueType;
    typedef KEY_TYPE KeyType;
    typedef typename StorageType::RecordType         RecordType;
    typedef typename StorageType::IteratorType       Iterator;
    typedef typename StorageType::ConstIteratorType  ConstIterator;

    //! Constructor.
    inline FbxMap() {}
    /** Copy constructor.
    * \param pMap The object to be copied.
    */
    inline FbxMap(const FbxMap& pMap) : mTree(pMap.mTree) {}
    //! Destructor.
    inline ~FbxMap() {Clear();}

    /** Preallocate memory.
    * \param pRecordCount The number of elements.
    */
    inline void Reserve(unsigned int pRecordCount)
    {
        mTree.Reserve(pRecordCount);
    }

    //! Retrieve the number of key-value pairs it holds.
    inline int GetSize() const
    {
        return mTree.GetSize();
    }

    /** Insert a key-value pair.
    * \param pKey The key.
    * \param pValue The value.
    * \return If the key is already present in the map, returns the existing pair
    *  and false; else returns the pointer to the new key-value and true.
    */
    inline FbxPair<RecordType*, bool> Insert(const KeyType& pKey, const ValueType& pValue)
    {
        return mTree.Insert(FbxKeyValuePair(pKey, pValue));
    }

    /** Delete a key-value pair.
    * \param pKey The key.
    * \return \c true if success, \c false if key is not found.
    */
    inline int Remove(const KeyType& pKey)
    {
        return mTree.Remove(pKey);
    }

    //! Clear the map.
    inline void Clear()
    {
        mTree.Clear();
    }

    //! Query whether the map is empty.
    inline bool Empty() const
    {
        return mTree.Empty();
    }

    //! Retrieve the begin iterator of the map.
    Iterator Begin()
    {
        return Iterator(Minimum());
    }

    //! Retrieve the end iterator of the map.
    Iterator End()
    {
        return Iterator();
    }

    //! Retrieve the begin iterator of the map.
    ConstIterator Begin() const
    {
        return ConstIterator(Minimum());
    }

    //! Retrieve the end iterator of the map.
    ConstIterator End() const
    {
        return ConstIterator();
    }

    /** Query a key.
    * \param pKey The key.
    * \return A key-value pair if success, NULL if the key is not found.
    */
    inline const RecordType* Find(const KeyType& pKey) const
    {
        return mTree.Find(pKey);
    }

    /** Query a key.
    * \param pKey The key.
    * \return A key-value pair if success, NULL if it's not found.
    */
    inline RecordType* Find(const KeyType& pKey)
    {
        return mTree.Find(pKey);
    }

    /** Find the key-value pair with the smallest key greater than a specified key.
    * \param pKey The key.
    * \return The found key-value pair.
    */
    inline const RecordType* UpperBound(const KeyType& pKey) const
    {
        return mTree.UpperBound(pKey);
    }

    /** Find the key-value pair with the smallest key greater than a specified key.
    * \param pKey The key.
    * \return The found key-value pair.
    */
    inline RecordType* UpperBound(const KeyType& pKey)
    {
        return mTree.UpperBound(pKey);
    }

    /** Retrieve the reference of the value in the key-value pairs in map.
    * \param pKey The key.
    * \return The reference of the value.
    * \remarks If the key is not found, a new key-value pair will be inserted.
    */
    inline ValueType& operator[](const KeyType& pKey)
    {
        RecordType* lRecord = Find(pKey);

        if( !lRecord )
        {
            lRecord = Insert(pKey, ValueType()).mFirst;
        }

        return lRecord->GetValue();
    }

    //! Retrieve the key-value pair which is the minimum key in map.
    inline const RecordType* Minimum() const
    {
        return mTree.Minimum();
    }

    //! Retrieve the key-value pair which is the minimum key in map.
    inline RecordType* Minimum()
    {
        return mTree.Minimum();
    }

    //! Retrieve the key-value pair which is the maximum key in map.
    inline const RecordType* Maximum() const
    {
        return mTree.Maximum();
    }

    //! Retrieve the key-value pair which is the maximum key in map.
    inline RecordType* Maximum()
    {
        return mTree.Maximum();
    }
};

/** This class implements an efficient set based on value comparison, which stores values.
  * It executes insertion, deletion and query operations in O(log(n)) time.
  */
template <typename VALUE_TYPE,
          typename KEY_COMPARE_FUNCTOR = FbxLessCompare<VALUE_TYPE>,
          typename ALLOCATOR = FbxBaseAllocator>
class FbxSet2
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
protected:
    class FbxValue
    {
    public:
        inline FbxValue(const VALUE_TYPE& pValue)
            : mValue(pValue) {}

        // Cannot modify the value in a set, which would change its ordering in the tree
        typedef const VALUE_TYPE KeyType;
        typedef const VALUE_TYPE ConstKeyType;
        typedef const VALUE_TYPE ValueType;
        typedef const VALUE_TYPE ConstValueType;

        inline KeyType& GetKey() const      { return mValue; }
        inline ConstKeyType& GetKey()       { return mValue; }

        inline ValueType& GetValue() const  { return mValue; }
        inline ConstValueType& GetValue()   { return mValue; }

    protected:
        ValueType mValue;

	private:
		FbxValue& operator=(const FbxValue&);
    };

    typedef FbxRedBlackTree<FbxValue, KEY_COMPARE_FUNCTOR, ALLOCATOR> StorageType;
    StorageType mTree;
#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS


public:
    typedef VALUE_TYPE ValueType;
    typedef typename StorageType::RecordType        RecordType;
    typedef typename StorageType::IteratorType      Iterator;
    typedef typename StorageType::ConstIteratorType ConstIterator;

    //! Constructor.
    inline FbxSet2() {}
    //! Copy constructor.
    inline FbxSet2(const FbxSet2& pSet) : mTree(pSet.mTree) {}
    //! Destructor.
    inline ~FbxSet2() {Clear();}

    /** Preallocate memory.
    * \param pRecordCount The number of elements.
    */
    inline void Reserve(unsigned int pRecordCount)
    {
        mTree.Reserve(pRecordCount);
    }

    //! Retrieve the number of values it holds.
    inline int GetSize() const
    {
        return mTree.GetSize();
    }

    /** Insert a value.
    * \param pValue The value.
    * \return If the value is already present in the map, returns the existing value
    *  and false; else returns the pointer to the new value and true.
    */
    inline FbxPair<RecordType*, bool> Insert(const ValueType& pValue)
    {
        return mTree.Insert(FbxValue(pValue));
    }

    /** Delete a value.
    * \param pValue The value.
    * \return \c true if success, \c false if value is not found.
    */
    inline int Remove(const ValueType& pValue)
    {
        return mTree.Remove(pValue);
    }

    //! Clear the set.
    inline void Clear()
    {
        mTree.Clear();
    }

    //! Query whether the set is empty.
    inline bool Empty() const
    {
        return mTree.Empty();
    }

    //! Retrieve the begin iterator of the set.
    Iterator Begin()
    {
        return Iterator(Minimum());
    }

    //! Retrieve the end iterator of the set.
    Iterator End()
    {
        return Iterator();
    }

    //! Retrieve the begin iterator of the set.
    ConstIterator Begin() const
    {
        return ConstIterator(Minimum());
    }

    //! Retrieve the end iterator of the set.
    ConstIterator End() const
    {
        return ConstIterator();
    }

    /** Query a value in the set.
    * \param pKey The key.
    * \return The key if success, NULL if it's not found.
    */
    inline const RecordType* Find(const ValueType& pValue) const
    {
        return mTree.Find(pValue);
    }

    /** Query a value in the set.
    * \param pKey The key.
    * \return The key if success, NULL if it's not found.
    */
    inline RecordType* Find(const ValueType& pValue)
    {
        return mTree.Find(pValue);
    }

    //! Retrieve the minimum value in the set.
    inline const RecordType* Minimum() const
    {
        return mTree.Minimum();
    }

    //! Retrieve the minimum value in the set.
    inline RecordType* Minimum()
    {
        return mTree.Minimum();
    }

    //! Retrieve the maximum value in the set.
    inline const RecordType* Maximum() const
    {
        return mTree.Maximum();
    }

    //! Retrieve the maximum value in the set.
    inline RecordType* Maximum()
    {
        return mTree.Maximum();
    }

    //! Equality operator.
    inline bool operator==(const FbxSet2<VALUE_TYPE, KEY_COMPARE_FUNCTOR, ALLOCATOR>& pOther) const
    {
        return (this == &pOther) || (mTree == pOther.mTree);
    }

    //! Inequality operator.
    inline bool operator != (const FbxSet2<VALUE_TYPE, KEY_COMPARE_FUNCTOR, ALLOCATOR>& pOther) const
    {
        return !(*this == pOther);
    }

    /** Intersect with another set.
    * \param The other set.
    * \return The intersection set of the two sets.
    */
    inline FbxSet2 Intersect(const FbxSet2& pOther) const
    {
        FbxSet2 lReturn;
        ConstIterator lBegin = Begin();
        for (; lBegin != End(); ++lBegin)
        {
            if (pOther.Find(lBegin->GetValue()) != NULL)
                lReturn.Insert(lBegin->GetValue());
        }
        return lReturn;
    }

    /** Unite with another set.
    * \param The other set.
    * \return The union set of the two sets (no duplicated items).
    */
    inline FbxSet2 Union(const FbxSet2& pOther) const
	{
		FbxSet2 lReturn(*this);
		ConstIterator lBegin = pOther.Begin();
		for (; lBegin != End(); ++lBegin)
		{
			if (Find(lBegin->GetValue()) == NULL)
				lReturn.Insert(lBegin->GetValue());
		}
		return lReturn;
	}

};

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_CORE_BASE_MAP_H_ */
