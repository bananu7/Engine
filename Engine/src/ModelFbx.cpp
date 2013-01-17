
#include <fbxsdk.h>
#include "ModelFbx.h"
#include "FbxVboMesh.h"
#include "FbxAnimHelpers.h"
#include "Camera.h"

using std::string;

void CModelFbx::_InitializeFbxComponents()
{	
	if (FbxManagerInstance == NULL)
		FbxManagerInstance = FbxManager::Create();						// create a FBX SdkManager

	if (FbxIOSettingsInstance == NULL)
		FbxIOSettingsInstance = FbxIOSettings::Create(FbxManagerInstance, IOSROOT );	// create a FBX IOSettings object
	
	// set some IOSettings options 
	FbxIOSettingsInstance->SetBoolProp(IMP_FBX_MATERIAL, true);
	FbxIOSettingsInstance->SetBoolProp(IMP_FBX_TEXTURE,  true);
	FbxManagerInstance->SetIOSettings(FbxIOSettingsInstance);

	if (FbxImporterInstance == NULL)
		FbxImporterInstance = FbxImporter::Create(FbxManagerInstance, "");	
}

std::string CModelFbx::Load(ILoader const& loadParams)
{
	_InitializeFbxComponents();

	m_Scene = FbxScene::Create(FbxManagerInstance, "");
	std::string Path;
	loadParams.GetParam("path", Path);
	if (!FbxImporterInstance->Initialize(Path.c_str(), -1, FbxIOSettingsInstance))
		return string(FbxImporterInstance->GetLastErrorString());
	FbxImporterInstance->Import(m_Scene);
	
	int MeshCount = m_Scene->GetGeometryCount();
	/*if (MeshCount > 1)
		return string("Only one Mesh per file accepted");
	else if (MeshCount == 0)
		return string("No Mesh data found in file");*/

	m_RawMesh = static_cast<FbxMesh*>(m_Scene->GetGeometry(0));

	//if (!m_RawMesh->IsTriangleMesh())
	//	return string("Mesh isn't triangulated");
	/*{
		FbxGeometryConverter Converter(FbxManagerInstance);
		bool Status;
		// Triangulujemy mesh
		m_RawMesh = Converter.TriangulateMesh(m_RawMesh);
		//if (!Status)
		//	return string("Error with mesh triangulation");
	}*/

	m_Mesh->Initialize(m_RawMesh);

	return string();
}

void CModelFbx::Draw()
{
	m_Mesh->BeginDraw();
	m_Mesh->Draw(0);
	m_Mesh->EndDraw();
}

void CModelFbx::Draw (const CVector3& pos, const CVector3& rot, const CVector3& scale)
{
	CMatrix4 PMat = CCamera::CreateTranslation(pos.X, pos.Y, pos.Z);
	CMatrix4 RMat = CCamera::CreateRotation(rot.X, rot.Y, rot.Z);
	CMatrix4 SMat = CCamera::CreateScale(scale.X, scale.Y, scale.Z);
	PMat.Mult(RMat);
	PMat.Mult(SMat);
	m_Shader->SetUniformMatrix4("ModelMatrix", PMat);
	Draw();
}

void CModelFbx::SetAnimationPose(int animStack, int animationLayer, float time)
{
    const int lVertexCount = m_RawMesh->GetControlPointsCount();
	FbxTime Time;
	Time.SetMilliSeconds(time * 1000.f);

    // No vertex to draw.
    if (lVertexCount == 0)
    {
        return;
    }

    // If it has some defomer connection, update the vertices position
    const bool lHasVertexCache = m_RawMesh->GetDeformerCount(FbxDeformer::eVertexCache) &&
        (static_cast<FbxVertexCacheDeformer*>(m_RawMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->IsActive();
    const bool lHasShape = m_RawMesh->GetShapeCount() > 0;
    const bool lHasSkin = m_RawMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
    const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;

	const int AnimStackCount = m_Scene->GetSrcObjectCount(FBX_TYPE(FbxAnimStack));
	FbxAnimStack* AnimStack = FbxCast<FbxAnimStack>(m_Scene->GetSrcObject(FBX_TYPE(FbxAnimStack), animStack));
	FbxAnimLayer* AnimLayer = AnimStack->GetMember(FBX_TYPE(FbxAnimLayer), animationLayer);

	FbxTime Stop = AnimStack->LocalStop;
	while (Time > Stop)
		Time -= Stop;

    FbxVector4* lVertexArray = NULL;
    if (lHasDeformation)
    {
        lVertexArray = new FbxVector4[lVertexCount];
        memcpy(lVertexArray, m_RawMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
    }

    if (lHasDeformation)
    {
        // Active vertex cache deformer will overwrite any other deformer
        if (lHasVertexCache)
        {
            ReadVertexCacheData(m_RawMesh, Time, lVertexArray);
        }
        else
        {
            if (lHasShape)
            {
                // Deform the vertex array with the shapes.
                ComputeShapeDeformation(m_RawMesh, Time, AnimLayer, lVertexArray);
            }

            //we need to get the number of clusters
            const int lSkinCount = m_RawMesh->GetDeformerCount(FbxDeformer::eSkin);
            int lClusterCount = 0;
            for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
            {
                lClusterCount += ((FbxSkin *)(m_RawMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin)))->GetClusterCount();
            }
            if (lClusterCount)
            {
                // Deform the vertex array with the skin deformer.
				FbxAMatrix DummyGlobalPosition;
                ComputeSkinDeformation(DummyGlobalPosition, m_RawMesh, Time, lVertexArray, /*pPose*/NULL);
            }
        }

        m_Mesh->UpdateVertexPosition(m_RawMesh, lVertexArray);
    }
}

CModelFbx::CModelFbx(void)
{
	m_Mesh = new CFbxVboMesh();
}


CModelFbx::~CModelFbx(void)
{
	delete m_Mesh;
}

FbxManager* CModelFbx::FbxManagerInstance;
FbxIOSettings* CModelFbx::FbxIOSettingsInstance;
FbxImporter* CModelFbx::FbxImporterInstance;