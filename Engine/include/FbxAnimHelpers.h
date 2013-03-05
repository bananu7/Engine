#pragma once

#ifdef ENGINE_USE_FBX

#include <fbxsdk.h>
#include "GetPosition.h"

void ComputeShapeDeformation(FbxMesh* pMesh, 
                             FbxTime& pTime, 
                             FbxAnimLayer * pAnimLayer,
                             FbxVector4* pVertexArray);
void ComputeClusterDeformation(FbxAMatrix& pGlobalPosition, 
							   FbxMesh* pMesh,
							   FbxCluster* pCluster, 
							   FbxAMatrix& pVertexTransformMatrix,
							   FbxTime pTime, 
							   FbxPose* pPose);
void ComputeLinearDeformation(FbxAMatrix& pGlobalPosition, 
							  FbxMesh* pMesh, 
							  FbxTime& pTime, 
							  FbxVector4* pVertexArray,
							  FbxPose* pPose);
void ComputeDualQuaternionDeformation(FbxAMatrix& pGlobalPosition, 
									  FbxMesh* pMesh, 
									  FbxTime& pTime, 
									  FbxVector4* pVertexArray,
									  FbxPose* pPose);
void ComputeSkinDeformation(FbxAMatrix& pGlobalPosition, 
							FbxMesh* pMesh, 
							FbxTime& pTime, 
							FbxVector4* pVertexArray,
							FbxPose* pPose);

void ReadVertexCacheData(FbxMesh* pMesh, 
                         FbxTime& pTime, 
                         FbxVector4* pVertexArray);

void MatrixScale(FbxAMatrix& pMatrix, double pValue);
void MatrixAddToDiagonal(FbxAMatrix& pMatrix, double pValue);
void MatrixAdd(FbxAMatrix& pDstMatrix, FbxAMatrix& pSrcMatrix);

} // namespace engine

#endif