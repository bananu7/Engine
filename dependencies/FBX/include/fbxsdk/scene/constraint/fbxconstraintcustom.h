/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxconstraintcustom.h
#ifndef _FBXSDK_SCENE_CONSTRAINT_CUSTOM_H_
#define _FBXSDK_SCENE_CONSTRAINT_CUSTOM_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/scene/constraint/fbxconstraint.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

class FbxManager;

/** \brief This constraint class contains methods for custom constraint.
  * \nosubgrouping
  */
class FBXSDK_DLL FbxConstraintCustom : public FbxConstraint
{
    FBXSDK_OBJECT_DECLARE(FbxConstraintCustom, FbxConstraint);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
protected:
    FbxConstraintCustom(FbxManager& pManager, char const* pName);
    virtual EType GetConstraintType() const;
#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS
};

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_SCENE_CONSTRAINT_CUSTOM_H_ */
