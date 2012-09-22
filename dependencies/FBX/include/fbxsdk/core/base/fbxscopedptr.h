/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxscopedptr.h
#ifndef _FBXSDK_CORE_BASE_SCOPED_PTR_H_
#define _FBXSDK_CORE_BASE_SCOPED_PTR_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

/**
* A deletion policy for pointer template classes that uses the delete operator.
* By default KScopePtr uses FbxDefaultDeletionPolicy.
*/
template<class T> class FbxDefaultDeletionPolicy
{
public:
    static inline void DeleteIt(T** ptr)
    {
        if ( *ptr != NULL )
        {
            delete *ptr;
            *ptr = NULL;
        }
    }
};

/**
* A deletion policy that uses the FbxFree() function instead of the delete operator.
*/
template<class T> class FbxFreeDeletionPolicy
{
public:
    static inline void DeleteIt(T** ptr)
    {
        if ( *ptr != NULL )
        {
            FbxFree( *ptr );
            *ptr = NULL;
        }
    }
};

/** FbxScopedPtr mimics a built-in pointer except that it guarantees deletion
* of the object pointed to, either on destruction of the FbxScopedPtr or via
* an explicit Reset().
*/
template<class T, class DeletionPolicyT = FbxDefaultDeletionPolicy<T> > class FbxScopedPtr
{
private:
    T* ptr;

    // Non copyable object
    FbxScopedPtr(const FbxScopedPtr&);
    FbxScopedPtr& operator=(const FbxScopedPtr&);

    typedef FbxScopedPtr<T, DeletionPolicyT> ThisType;
    typedef DeletionPolicyT DeletionPolicy;

public:
    //! Construct from a pointer.
    explicit FbxScopedPtr(T* p = 0): ptr(p){}

    //! Destructor.
    ~FbxScopedPtr()
    {
        DeletionPolicy::DeleteIt(&ptr);
    }

    //! Reset the scoped pointer by swapping with another pointer.
    inline void Reset(T* p = 0)
    {
        FBX_ASSERT(p == 0 || p != ptr); // catch self-reset errors
        ThisType(p).Swap(*this);
    }

    //! Dereference operator.
    inline T& operator*() const
    {
        FBX_ASSERT(ptr != 0);
        return *ptr;
    }

    //! Member access operator.
    inline T* operator->() const
    {
        FBX_ASSERT(ptr != 0);
        return ptr;
    }

    //! Retrieve the pointer it holds.
    inline T* Get() const
    {
        return ptr;
    }

    //! Convert to a type T pointer.
    inline operator T* () const
    {
        return ptr;
    }

    //! Convert to boolean value.
    operator bool () const
    {
        return ptr != 0;
    }

    //! Logical not operator.
    bool operator! () const
    {
        return ptr == 0;
    }

    //! Swap with another pointer.
    void Swap(FbxScopedPtr & b)
    {
        T * tmp = b.ptr;
        b.ptr = ptr;
        ptr = tmp;
    }

    //! Release the pointer, so that it won't perform deletion in its destruction.
    T* Release()
    {
        T* tmp = ptr;
        ptr = NULL;

        return tmp;
    }
};

/**
* A deletion policy for pointer template classes that uses the Destroy() function.
* This policy is used by FbxObjectScopedPtr.
*/
template <class FBXObjectT> class FbxObjectDeletionPolicy
{
public:
    static inline void DeleteIt(FBXObjectT** ptr)
    {
        if (*ptr != NULL)
        {
            (*ptr)->Destroy();
            *ptr = NULL;
        }
    }
};

//! Scoped pointer for FbxObject, which call Destroy() to destroy the object.
template <class FBXObjectT> class FbxObjectScopedPtr: public FbxScopedPtr<FBXObjectT, FbxObjectDeletionPolicy<FBXObjectT> >
{
public:
    explicit FbxObjectScopedPtr(FBXObjectT* p = 0):FbxScopedPtr<FBXObjectT, FbxObjectDeletionPolicy<FBXObjectT> >(p){}
};

/**
* A deletion policy for pointer template classes that uses the FbxSdkDelete() function.
*/
template <class T> class FbxDeletionPolicy
{
public:
    static inline void DeleteIt(T** ptr)
    {
        if (*ptr != NULL)
        {
            FbxDelete(*ptr);
            *ptr = NULL;
        }
    }
};

/** FbxDefaultScopedPtr mimics a built-in pointer except that it guarantees deletion
* of the object pointed to, either on destruction of the FbxScopedPtr or via
* an explicit Reset().
* \remarks Its deletion policy is FbxDeletionPolicy not FbxDefaultDeletionPolicy.
*/
template <class T> class FbxDefaultScopedPtr: public FbxScopedPtr<T, FbxDeletionPolicy<T> >
{
public:
    explicit FbxDefaultScopedPtr(T* p = 0):FbxScopedPtr<T, FbxDeletionPolicy<T> >(p){}
};

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_CORE_BASE_SCOPED_PTR_H_ */
