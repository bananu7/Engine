/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxutilities.h
#ifndef _FBXSDK_UTILS_UTILITIES_H_
#define _FBXSDK_UTILS_UTILITIES_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/scene/fbxscene.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

class FbxTexture;

/** 
	* \name Scene cleaning
	*/
//@{

/** Clean bad meshes in the scene by removing bad polygons.
	* \param pScene     Pointer to the scene object.
	* \return           Array of nodes containing modified meshes.
	*/
FBXSDK_DLL FbxArray<FbxNode*> RemoveBadPolygonsFromMeshes(FbxScene *pScene);
//@}


/** 
	* \name Animation query functions
	*/
//@{

/** Inspects all the properties of the given object for animation curves.
  * \param pObj Pointer to the object to query.
  * \return     \c true if at least one property is animated and \c false otherwise.
  * \remarks    A property is animated if it contains at least one FbxAnimCurve with keys.
  */
FBXSDK_DLL bool IsAnimated(FbxObject* pObj);

/** Inspects the specified property of the given object for animation curves.
  * \param pObj             Pointer to the object to query.
  * \param pPropertyName    Name of the inspected property.
  * \param pChannelName     Name of the specific channel of the inspected property.
  * \return                 \c true if the specified channel is animated and \c false otherwise.
  * \remarks                A property is animated if it contains at least one FbxAnimCurve with keys.
  */
FBXSDK_DLL bool IsChannelAnimated(FbxObject* pObj, const char* pPropertyName, const char* pChannelName = NULL);
//@}

/** 
  * \name File utilities
  */
//@{

/** Get the content of a file from an hyper link
  * \param url The hyper link to the file. The URL must start with < http:// > ex: http://www.autodesk.com.
  * \return An allocated <char *buffer> containing the source text. You must use FBXFree_Internet_Buffer() to 
  * free the allocated memory.
  */
FBXSDK_DLL char* FBXGet_Internet_Source_File(const char *url);

/** Free the memory allocated by FBXGet_Internet_Source_File()
  * \param pBuf Pointer previously returned by a call to FBXGet_Internet_Source_File().
  */
FBXSDK_DLL void FBXFree_Internet_Buffer(char* pBuf);

/** Get the application directory
  * \return The application directory.  
  */
FBXSDK_DLL FbxString FbxGetApplicationDirectory();

//@}

/** 
  * \name File Version utility
  */
//@{

/** Convert the FBX file version string to an integral number for <= or >= tests purposes.
  * \param pFileVersion File version string.
  * Some examples:
  * \code
  *     int version;
  *         version = FileVersionStrToInt(FBX2012_00_COMPATIBLE);    // version = 201200
  *         version = FileVersionStrToInt(FBX60_COMPATIBLE);         // version = 6000
  *         version = FileVersionStrToInt("FBX200900");              // version = 200900
  *         version = FileVersionStrToInt("Toto");                   // version = 0
  * \endcode
  * \returns the file version number or 0 if an unsupported string value is passed.
  */

FBXSDK_DLL unsigned int FileVersionStrToInt(FbxString pFileVersion);

//@}

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_UTILS_UTILITIES_H_ */
