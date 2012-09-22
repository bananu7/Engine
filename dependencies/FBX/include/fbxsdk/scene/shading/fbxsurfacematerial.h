/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxsurfacematerial.h
#ifndef _FBXSDK_SCENE_SHADING_SURFACE_MATERIAL_H_
#define _FBXSDK_SCENE_SHADING_SURFACE_MATERIAL_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/core/fbxobject.h>
#include <fbxsdk/scene/fbxgroupname.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

/** This class contains material settings.
  * \nosubgrouping
  */
class FBXSDK_DLL FbxSurfaceMaterial : public FbxObject
{
	FBXSDK_OBJECT_DECLARE(FbxSurfaceMaterial, FbxObject);

public:
	/**
	  * \name Standard Material Property Names
	  */
	//@{	

	static char const* sShadingModel;
	static char const* sMultiLayer;
	
	static char const* sEmissive;
	static char const* sEmissiveFactor;
	
	static char const* sAmbient;
	static char const* sAmbientFactor;
	
	static char const* sDiffuse;
	static char const* sDiffuseFactor;
	
	static char const* sSpecular;
	static char const* sSpecularFactor;
	static char const* sShininess;
	
	static char const* sBump;
	static char const* sNormalMap;
    static char const* sBumpFactor;

	static char const* sTransparentColor;
	static char const* sTransparencyFactor;
	
	static char const* sReflection;
	static char const* sReflectionFactor;

    static char const* sDisplacementColor;
    static char const* sDisplacementFactor;

    static char const* sVectorDisplacementColor;
    static char const* sVectorDisplacementFactor;
	//@}	

	/**
	  * \name Material Properties
	  */
	//@{	
	FbxPropertyT<FbxString> ShadingModel;
	FbxPropertyT<FbxBool> MultiLayer;
	//@}	

	//////////////////////////////////////////////////////////////////////////
	// Static values
	//////////////////////////////////////////////////////////////////////////

	/**
	  * \name Default property values
	  */
	//@{

	static const FbxBool sMultiLayerDefault;
	static char const*	sShadingModelDefault;

    //@}

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
	bool SetColorParameter(FbxProperty pProperty, FbxColor const& pColor);
	bool GetColorParameter(FbxProperty pProperty, FbxColor& pColor) const;
	bool SetDoubleParameter(FbxProperty pProperty, double pDouble);
	bool GetDoubleParameter(FbxProperty pProperty, double pDouble) const;
	
	FbxSurfaceMaterial(FbxManager& pManager, char const* pName);

	virtual bool ConstructProperties(bool pForceSet);

protected:

	friend class FbxLayerContainer;
#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS 
};

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_SCENE_SHADING_SURFACE_MATERIAL_H_ */
