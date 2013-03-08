#include "Physics.h"
#include <PxToolkit.h>
#include <string>
#include <fbxsdk.h>
#include "Vector3.h"

using namespace std;
using namespace physx;

#pragma pack (push, 1) 
struct Vector3 { float X, Y, Z; Vector3() { } };
struct Vector2 { float U, V;  };
#pragma pack (pop)

bool CPhysics::LoadTriangleMesh(std::string const& path, CVector3 const& pos)
{
	FbxManager *m_SdkManager= FbxManager::Create();						// create a FBX SdkManager
	FbxIOSettings *ios= FbxIOSettings::Create(m_SdkManager, IOSROOT );	// create a FBX IOSettings object
	
	// set some IOSettings options 
	ios->SetBoolProp(IMP_FBX_MATERIAL, false);
	ios->SetBoolProp(IMP_FBX_TEXTURE,  true);

	FbxScene* m_Scene= FbxScene::Create(m_SdkManager,"");				// create an empty scene
	FbxImporter* m_Importer= FbxImporter::Create(m_SdkManager, "");		// create an importer.
	m_Importer->Initialize(path.c_str(), -1, ios);						// initialize the importer by providing a filename and the IOSettings to use
																		// -1 means autodetect filetype
	m_Importer->Import(m_Scene);										// import the scene.
	
	// Wczytywanie Meshy
	string						m_ComponentName;
	int							m_ComponentNumber(m_Scene->GetGeometryCount());

	FbxGeometry					*m_Geometry;
	FbxVector4					*ControlPointArray;						// get size and pointer to an array containing all the geometrie
	int							ControlPointSize;

	int							PolygonCount;
	int*						Indices;
	float*						Vertices;

	for (int it= 0, itE= m_ComponentNumber; it<itE; ++it)				// foreach in the array
	{
		Vector3 Vert;
		m_Geometry			= m_Scene->GetGeometry(it);
		FbxMesh* m_Mesh = static_cast<FbxMesh*>(m_Geometry);

		if (m_Mesh == NULL) // Nie wspieramy (na razie) ladowania innych geometrii niz meshe
			return false;

		FbxGeometryConverter Converter(m_SdkManager);
		bool Status;
		// Triangulujemy mesh
		m_Mesh = Converter.TriangulateMeshAdvance(m_Mesh, Status);
		if (!Status)
			return false;

		ControlPointArray	= m_Geometry->GetControlPoints();			// get size and pointer to an array containing all the CP within a geometry
		ControlPointSize	= m_Geometry->GetControlPointsCount();
		Vertices = new float[ControlPointSize * 3];

		#pragma region Loading of polygons
		PolygonCount = m_Mesh->GetPolygonCount();
		Indices = new int[PolygonCount * 3];
		for (int polygon = 0; polygon < PolygonCount; ++polygon)
		{
			int lPolygonSize = m_Mesh->GetPolygonSize(polygon);
			Indices[polygon * 3 + 0] = m_Mesh->GetPolygonVertex(polygon, 0);
			Indices[polygon * 3 + 1] = m_Mesh->GetPolygonVertex(polygon, 1);
			Indices[polygon * 3 + 2] = m_Mesh->GetPolygonVertex(polygon, 2);
			/*if (lPolygonSize == 3) // accept only triangles
			{
				for (int j = 0; j < lPolygonSize; j++)
				{
					int lControlPointIndex = m_Mesh->GetPolygonVertex(polygon, j);
				}
			}*/
		}
		#pragma endregion

		for (int itCP = 0; itCP < ControlPointSize; ++itCP)	// foreach Control Point
		{
			// Wierzcholki
			Vertices[itCP * 3 + 0] = ControlPointArray[itCP][0];
			Vertices[itCP * 3 + 1] = ControlPointArray[itCP][1];
			Vertices[itCP * 3 + 2] = ControlPointArray[itCP][2];
		}
	}
	m_Importer->Destroy();

	// PhysX 
	physx::PxTriangleMeshDesc meshDesc;
	meshDesc.points.count           = ControlPointSize;
	meshDesc.triangles.count        = PolygonCount;
	meshDesc.points.stride          = 4*3;
	meshDesc.triangles.stride       = 4*3;
	meshDesc.points.data            = Vertices;
	meshDesc.triangles.data         = Indices;

	//PxCooking* cooking = PxCreateCooking(PX_PHYSICS_VERSION, , physx::PxCookingParams());


	PxToolkit::MemoryWriteBuffer buf;

	bool status = CPhysics::GetSingleton()->GetCooking()->cookTriangleMesh(meshDesc, buf);
	physx::PxTriangleMesh* triangleMesh =
		CPhysics::GetSingleton()->GetSDK()->createTriangleMesh(PxToolkit::MemoryReadBuffer(buf.data));
	CPhysics::GetSingleton()->GetCooking()->release();

	PxTransform Transform (PxVec3 (pos.X, pos.Y, pos.Z));
	PxRigidStatic* aSphereActor = CPhysics::GetSingleton()->GetSDK()->createRigidStatic(Transform);
	PxShape* aSphereShape = aSphereActor->createShape(
		PxTriangleMeshGeometry(triangleMesh),
		*CPhysics::GetSingleton()->GetBaseMaterial()
		);

	CPhysics::GetSingleton()->GetScene()->addActor(*aSphereActor);

	delete Indices;
	delete Vertices;

	return true;
}