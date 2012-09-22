/****************************************************************************************
 
   Copyright (C) 2012 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxiobase.h
#ifndef _FBXSDK_FILEIO_IO_BASE_H_
#define _FBXSDK_FILEIO_IO_BASE_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/core/base/fbxerror.h>
#include <fbxsdk/core/base/fbxstring.h>

#include <fbxsdk/core/fbxobject.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

class FbxManager;

#define FBXSDK_IO_END_NODE_STR "_End"

/** \brief Base class for FBX file importer and exporter.
  * \nosubgrouping
  */
class FBXSDK_DLL FbxIOBase : public FbxObject
{
    FBXSDK_OBJECT_DECLARE(FbxIOBase, FbxObject);

public:
    /** Initializes the object.
      * \param pFileName     The name of the file to access.
      * \return              \c True if successful, returns \c false otherwise.
      * \remarks             To identify the error, call FbxIOBase::GetLastErrorID().
      */
    virtual bool Initialize(const char *pFileName);

    /** Returns the file name.
       * \return     The file name or an empty string if no filename has been set.
       */
    virtual FbxString GetFileName();

    /**
      * \name Error Management
      */
    //@{

    /** Retrieves the error object.
      * \return     A reference to the error object.
      */
    FbxError& GetError();

    /** \enum EErrorCode Error identifiers.
      * - \e eFileCorrupted
      * - \e eFileVersionNotSupportedYet
      * - \e eFileVersionNotSupportedAnymore
      * - \e eFileNotOpened
      * - \e eFileNotCreated
      * - \e eOutOfSpace
      * - \e eUninitializedFileName
      * - \e eUnknownError
      * - \e eIndexOutOfRange
      * - \e ePasswordError
      * - \e eEmbeddedOutOfSpace
      */
    enum EErrorCode
    {
        eFileCorrupted,
        eFileVersionNotSupportedYet,
        eFileVersionNotSupportedAnymore,
        eFileNotOpened,
        eFileNotCreated,
        eOutOfSpace,
        eUninitializedFileName,
        eUnknownError,
        eIndexOutOfRange,
        ePasswordError,
        eEmbeddedOutOfSpace,
        eErrorCount
    };

    /** Returns the last error code.
      * \return     The last error code.
      */
    EErrorCode GetLastErrorID() const;

    /** Returns the last error string.
      * \return     A textual description of the last error.
      */
    const char* GetLastErrorString() const;

    /** Returns the warning message from the file Reader and Writer.
      * \param pMessage     The warning message.
      */
    void GetMessage(FbxString& pMessage) const;

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

    FbxIOBase(FbxManager& pManager,char const* pName);

    int DetectReaderFileFormat(const char *pFileName);
    int DetectWriterFileFormat(const char *pFileName);

    FbxError mError;
    FbxString mFilename;
    FbxManager* mManager;
    FbxString mMessage;

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

};

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_FILEIO_IO_BASE_H_ */
