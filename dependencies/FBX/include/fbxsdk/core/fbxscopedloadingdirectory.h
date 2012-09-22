/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxscopedloadingdirectory.h
#ifndef _FBXSDK_CORE_SCOPED_LOADING_DIRECTORY_H_
#define _FBXSDK_CORE_SCOPED_LOADING_DIRECTORY_H_

#include <fbxsdk/fbxsdk_def.h>

// Local includes
#include <fbxsdk/core/fbxloadingstrategy.h>

// FBX includes
#include <fbxsdk/core/base/fbxstring.h>
#include <fbxsdk/core/fbxmodule.h>


#include <fbxsdk/fbxsdk_nsbegin.h>

class FbxPluginHandle;

/** 
 * A plug-in loading strategy that loads all DLLs with a specific extension from a specific directory.
 * When this class is destroyed all of the plug-ins are unloaded.
 */
class FBXSDK_DLL FbxScopedLoadingDirectory : public FbxLoadingStrategy
{
public:
    /**
     *\name Public interface
     */
    //@{

    /** Constructor. Load plug-ins.
	 * \param pDirectoryPath The directory path.
	 * \param pPluginExtension The plug-in extension.
     */
    FbxScopedLoadingDirectory(const char* pDirectoryPath, const char* pPluginExtension);

    /** Destructor. Unload plug-ins.
    */
    virtual ~FbxScopedLoadingDirectory();

    //@}

private:
    ///////////////////////////////////////////////////////////////////////////////
    //
    //  WARNING!
    //
    //	Anything beyond these lines may not be documented accurately and is 
    // 	subject to change without notice.
    //
    ///////////////////////////////////////////////////////////////////////////////
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    FbxString mDirectoryPath;
    FbxString mExtension;

    // From FbxLoadingStrategy
    virtual bool SpecificLoad(FbxPluginData& pData);
    virtual void SpecificUnload();

    // Plug-in management
    typedef FbxIntrusiveList<FbxPluginHandle> FbxPluginHandleList;
    FbxPluginHandleList mPluginHandles;

#endif
};


#ifndef DOXYGEN_SHOULD_SKIP_THIS
class FbxPluginHandle
{
	FBXSDK_INTRUSIVE_LIST_NODE(FbxPluginHandle, 1);

public:
	FbxPluginHandle(FbxModule pInstance=NULL) : mInstance(pInstance){}
	FbxModule mInstance;
};
#endif //DOXYGEN_SHOULD_SKIP_THIS

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_CORE_SCOPED_LOADING_DIRECTORY_H_ */
