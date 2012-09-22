/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxgenericnode.h
#ifndef _FBXSDK_SCENE_GEOMETRY_GENERIC_NODE_H_
#define _FBXSDK_SCENE_GEOMETRY_GENERIC_NODE_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/core/base/fbxerror.h>
#include <fbxsdk/core/fbxobject.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

class FbxManager;

/** Empty node containing properties.
  * \nosubgrouping
  */
class FBXSDK_DLL FbxGenericNode : public FbxObject
{
    FBXSDK_OBJECT_DECLARE(FbxGenericNode,FbxObject);

public:
        /**
          * \name Error Management
          */
        //@{

        /** Retrieve error object.
          * \return Reference to error object.
          */
        FbxError& GetError();

        /** \enum EErrorCode Error identifiers.
          * - \e eError
          * - \e eErrorCount
          */
        enum EErrorCode
        {
            eError,
            eErrorCount
        };

        /** Get last error code.
          * \return     Last error code.
          */
        EErrorCode GetLastErrorID() const;

        /** Get last error string.
          * \return     Textual description of the last error.
          */
        const char* GetLastErrorString() const;

        //@}

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
    FbxGenericNode(FbxManager& pManager, char const* pName);
    virtual FbxStringList GetTypeFlags() const;

    friend class FbxScene;

private:
    FbxError mError;

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS
};

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_SCENE_GEOMETRY_GENERIC_NODE_H_ */
