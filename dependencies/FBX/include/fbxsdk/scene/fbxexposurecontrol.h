/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxexposurecontrol.h
#ifndef _FBXSDK_SCENE_EXPOSURE_CONTROL_H_
#define _FBXSDK_SCENE_EXPOSURE_CONTROL_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/core/fbxobject.h>
#include <fbxsdk/core/fbxmanager.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

/** This object contains the properties of exposure control. It contains the properties of 
  * exposure mode, highlights, mid-tones, shadows, white point and saturation.
  * \nosubgrouping
  */
class FBXSDK_DLL FbxExposureControl : public FbxObject
{
	FBXSDK_OBJECT_DECLARE(FbxExposureControl,FbxObject);

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
	FbxExposureControl(FbxManager& pManager, char const* pName);
#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS
};

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_SCENE_EXPOSURE_CONTROL_H_ */
