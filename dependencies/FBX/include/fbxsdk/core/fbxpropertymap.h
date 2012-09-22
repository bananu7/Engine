/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxpropertymap.h
#ifndef _FBXSDK_CORE_PROPERTY_MAP_H_
#define _FBXSDK_CORE_PROPERTY_MAP_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/core/base/fbxmap.h>
#include <fbxsdk/core/fbxproperty.h>
#include <fbxsdk/core/fbxobject.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

/** A simple map class representing a dictionary-like data structure.
  * \nosubgrouping
  */
template <class K, class T, class Compare> class FBXSDK_DLL FbxMapExt
{
///////////////////////////////////////////////////////////////////////////////
//
//  WARNING!
//
//	Anything beyond these lines may not be documented accurately and is 
// 	subject to change without notice.
//
///////////////////////////////////////////////////////////////////////////////
#ifndef DOXYGEN_SHOULD_SKIP_THIS
private:
    typedef FbxMap<K, T, Compare> FbxMapExtDef;
    FbxMapExtDef mMap;
#endif // DOXYGEN_SHOULD_SKIP_THIS

public:
    typedef typename FbxMapExtDef::RecordType* Iterator;

    //! Constructor
    inline FbxMapExt()
    {
    }

    /** Add a key-value pair as an element.
      * \param pKey The new key.
      * \param pValue The new value.
      */
    inline void Add(const K& pKey, const T& pValue)
    {
        mMap.Insert(pKey, pValue);
    }

    /** Find an element with a given key.
      * \param pKey The given key.
      * \return The iterator pointing to the found element or NULL if fails.
      */
    inline Iterator Find(const K& pKey) const
    {
        return (Iterator)mMap.Find( pKey );
    }

    /** Find an element with a given value.
      * \param pValue The given value.
      * \return The iterator pointing to the found element or NULL if fails.
      */
    inline Iterator Find(const T& pValue) const
    {
        Iterator lIterator = GetFirst();
        while (lIterator)
        {
            if (lIterator->GetValue()==pValue)
            {
                return lIterator;
            }
            lIterator = GetNext(lIterator);
        }
        return 0;
    }

    /** Remove an element from the map.
      * \param pIterator The given element.
      */
    inline void Remove(Iterator pIterator)
    {
        if (pIterator) mMap.Remove( pIterator->GetKey() );
    }

    /** Get the first element.
      * \return The the heading element.
      */
    inline Iterator GetFirst() const
    {
        return (Iterator)mMap.Minimum();
    }

    /** Get the next element of a given element.
      * \param The given element.
      * \return The next element.
      */
    inline Iterator GetNext(Iterator pIterator) const
    {
        return (Iterator)pIterator ? pIterator->Successor() : 0;
    }

    /** Remove all of the elements.
      */
    inline void Clear() 
    {
        mMap.Clear();
    }

    /** Reserve the space for given number elements.
      * \param pSize The given number.
      */
    inline void Reserve(int pSize)
    {
        mMap.Reserve( pSize );
    }

    /** Query the count of elements in the map.
      * \return The count of elements.
      */
    inline int GetCount() const
    {
        return mMap.GetSize();
    }
};

/** This class maps types to objects.
  * \nosubgrouping
  */
template <class T, class Compare> class FBXSDK_DLL FbxObjectMap : public FbxMapExt<T,FbxObject*,Compare>
{
public:
    //! Constructor
    inline FbxObjectMap()
    {
    }

    /** Get the object contained in an element.
      * \param pIterator The given element.
      * \return The object.
      */
    inline FbxObject* Get(typename FbxMapExt<T,FbxObject*,Compare>::Iterator pIterator)
    {
        return pIterator ? pIterator->GetValue() : 0;
    }
};

/** This class is used to compare strings.
  * \nosubgrouping
  */
class FbxMapFbxStringCompare
{
public:
    /** Functor used to compared two strings.
      * \pKeyA The left string.
      * \pKeyB The right string.
      * \return If the left string is greater than the right, return 1; If the left string is less than the right, return -1; Or return 0 if both are false.
      */
    inline int operator()(const FbxString& pKeyA, const FbxString& pKeyB) const
    {
        return (pKeyA < pKeyB) ? -1 : ((pKeyB < pKeyA) ? 1 : 0);
    }
};

/** This class maps name strings to objects.
  * \nosubgrouping
  */
class FBXSDK_DLL FbxObjectStringMap : public FbxObjectMap<class FbxString,class FbxMapFbxStringCompare>
{
public:
    //! Constructor
    inline FbxObjectStringMap()
    {
    }
};

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_CORE_PROPERTY_MAP_H_ */
