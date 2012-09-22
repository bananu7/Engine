/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxobjectmetadata.h
#ifndef _FBXSDK_SCENE_OBJECT_META_DATA_H_
#define _FBXSDK_SCENE_OBJECT_META_DATA_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/core/fbxobject.h>
#include <fbxsdk/scene/fbxgroupname.h>

#include <fbxsdk/core/base/fbxstring.h>
#include <fbxsdk/core/base/fbxarray.h>
#include <fbxsdk/utils/fbxnamehandler.h>
#include <fbxsdk/core/math/fbxmatrix.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

/** This class is used to hold meta-data information on nodes.
  * \nosubgrouping
  *
  * This class does not offer any new functionality over a regular FbxObject;
  * all meta-data information should be stored in properties.
  * 
  */
class FBXSDK_DLL FbxObjectMetaData : public FbxObject
{
	FBXSDK_OBJECT_DECLARE(FbxObjectMetaData,FbxObject);

public:
///////////////////////////////////////////////////////////////////////////////
//
//  WARNING!
//
//	Anything beyond these lines may not be documented accurately and is 
// 	subject to change without notice.
//
///////////////////////////////////////////////////////////////////////////////
#ifndef DOXYGEN_SHOULD_SKIP_THIS
protected:
	/** Create an object of type FbxObjectMetaData.
	 * \param pManager (need to exist for extended validation).
	 * \param pName Name of this FbxObjectMetaData.
	 */
	FbxObjectMetaData(FbxManager& pManager, char const* pName);
#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS 
};

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_SCENE_OBJECT_META_DATA_H_ */
