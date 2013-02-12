#include <string>
#include <fstream>

#include <fbxsdk.h>

#include "ModelData.h"
#include "ResManager.h"
#include "Image.h"
#include <glm/glm.hpp>

using namespace std;
using std::string;

using glm::vec3;

bool SkipSpaces (istream& str)
{
	char C = str.peek();
	while (C == ' ')
	{
		str.get();
		C = str.peek();
	}

	if (C == '\n' || C == '\r')
		return false;  // Line ends
	else
		return true; // There is still something we can read
}

bool CModelData::_LoadMtl (std::istream& str)
{
	bool HaveMat = false;
	string MatName;
	SMaterial TempMat;
	string MatCmd;
	do
	{
		MatCmd = "";
		str >> MatCmd;
		if (MatCmd.empty())
			str.ignore (1000, '\n');
		else if (MatCmd[0] == '#')
			str.ignore (1000, '\n');

		else if (MatCmd == "newmtl")
		{
			if (HaveMat)
			{
				m_Materials.insert(
					make_pair(MatName, TempMat));
			}
			else
				HaveMat = true;
			str >> MatName;
		}
		else if (MatCmd == "Ka")
		{
			str >> (TempMat.Ambient[0])
					>> (TempMat.Ambient[1]) 
					>> (TempMat.Ambient[2]);

			if (SkipSpaces(str))
				str >> (TempMat.Ambient[3]);
			else
				TempMat.Ambient[3] = 1.f;
		}
		else if (MatCmd == "Kd")
		{
			str >> (TempMat.Diffuse[0])
					>> (TempMat.Diffuse[1])
					>> (TempMat.Diffuse[2]);

			if (SkipSpaces(str))
				str >> (TempMat.Diffuse[3]);
			else
				TempMat.Diffuse[3] = 1.f;
		}
		else if (MatCmd == "Ks")
		{
			str >> (TempMat.Specular[0]) 
					>> (TempMat.Specular[1]) >> (TempMat.Specular[2]);
			if (SkipSpaces(str))
				str >> (TempMat.Specular[3]);
			else
				TempMat.Specular[3] = 1.f;
		}
		else if (MatCmd == "map_Kd")
		{
			string TexPath;
			str >> TexPath;
			// FIXME
			//CResManager::GetSingleton()->AddCatalogEntry(MatName, ILoader(TexPath));
			//TempMat.Textures.push_back(CResManager::GetSingleton()->GetResource<CImage>(MatName)->GetTexture());
			// Below is called just to f*cking update pixels -.-
			//CResManager::GetSingleton()->GetResource<CImage>(MatName)->Bind();
		}
	} while (!(str.eof() || MatCmd == ""));
	if (HaveMat)
	{
		m_Materials.insert(make_pair(MatName, TempMat));
	}

	return true;
}

bool CModelData::_LoadFromObj(std::istream& str)
{
	//str >> skipws;
	string Cmd;
	SVector3 Temp;
	SVector2 Temp2;

	// Ladowanie odbywa sie zawsze do ostatniego utworzonego komponentu i grupy
	// Jesli ich brakuje, sa automatycznie tworzone

	do
	{
		Cmd = "";
		str >> Cmd;
		if (Cmd == "#")
		{
			str.ignore(1000, '\n');
		}
		else if (Cmd == "mtllib")
		{
			string MatFileName;
			str >> MatFileName;
			// FIXME
			ifstream MatFile;
			//ifstream MatFile (
			//	CResManager::GetSingleton()->GetResourcePath() + MatFileName);
			if (!MatFile.is_open())
				return false;
			_LoadMtl(MatFile);
		}
		else if (Cmd == "usemtl")
		{
			// Przechodzimy do nastepnej grupy
			string MatName;
			str >> MatName;

			if (m_Components.empty())
				m_Components.push_back(SComponent("Default"));

			m_Components.back().Groups.push_back(SGroup(MatName));
			//ListElems.push_back(new CMaterialTrigger(MatName));
		}
		else if (Cmd == "v")
		{
			str >> Temp.X >> Temp.Y >> Temp.Z;
			m_Vertices.push_back(Temp);
		}
		else if (Cmd == "vt")
		{
			str >> Temp2.U >> Temp2.V;
			m_TexCoords.push_back(Temp2);
		}
		else if (Cmd == "vn")
		{
			str >> Temp.X >> Temp.Y >> Temp.Z;
			m_Normals.push_back(Temp);
		}
		else if (Cmd == "f")
		{	
			char C;
			SFace TempFace;
			for (unsigned int i = 0; i < 3; ++i)
			{
				// Odejmujemy 1, bo w .obj sa numerowane od 1

				str >> TempFace.VertIndices[i];
				--TempFace.VertIndices[i];
				str.ignore(1); // "/"
				str >> TempFace.TexCIndices[i];
				--TempFace.TexCIndices[i];
				str.ignore(1); // "/"
				str >> TempFace.NormIndices[i];
				--TempFace.NormIndices[i];
				str.get(C);    // " " lub "\n"
			} 

			// Tworzymy komponent glowny, jesli jeszcze zaden nie istnieje
			if (m_Components.empty())
				m_Components.push_back(SComponent("Default"));

			// Brak materialu
			if (m_Components.back().Groups.empty())
			{
				SMaterial TempMat;
				TempMat.Ambient[0] = 0.3;
				TempMat.Ambient[1] = 0.3;
				TempMat.Ambient[2] = 0.3;
				TempMat.Ambient[3] = 0.3;

				TempMat.Diffuse[0] = 0.3;
				TempMat.Diffuse[1] = 0.3;
				TempMat.Diffuse[2] = 0.3;
				TempMat.Diffuse[3] = 0.3;

				TempMat.Specular[0] = 0.3;
				TempMat.Specular[1] = 0.3;
				TempMat.Specular[2] = 0.3;
				TempMat.Specular[3] = 0.3;
				m_Materials["Default"] = TempMat;
				
				m_Components.back().Groups.push_back(SGroup("Default"));
			}
			//assert(!m_Components.back().Groups.empty());

			m_Components.back().Groups.back().Faces.push_back(TempFace);

			// Linijka ponizej umozliwia wczytywanie do konca linii
			// Wylaczylem ja, bo obslugujemy tylko i wylacznie trojkaty
			//while (!((C == '\n') || (C == '\r') || (File.eof())));
			//ListElems.push_back(TempFace);
		}
		else if (Cmd == "g")
		{
			// Tworzymy nowy komponent
			string ComponentName;
			str >> ComponentName;
			m_Components.push_back(SComponent(ComponentName));
			//ListElems.push_back(new CGroupBeginning(GroupName));
		}
		else if (Cmd == "gc")
		{
			vec3 GroupCenter;
			// FIXME
			//str >> GroupCenter;
			//dynamic_cast<CGroupBeginning*>(ListElems.back())->SetCenter(GroupCenter);
		}
		else
		{
			str.ignore(1000, '\n');
		}
	} while (!str.eof());

	return true;
}

bool CModelData::_LoadFromFBX(const std::string &path)
{
/*	FbxManager *m_SdkManager= FbxManager::Create();						// create a FBX SdkManager
	FbxIOSettings *ios= FbxIOSettings::Create(m_SdkManager, IOSROOT );	// create a FBX IOSettings object
	
	// set some IOSettings options 
	ios->SetBoolProp(IMP_FBX_MATERIAL, true);
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
	FbxLayerElementUV			*m_TexCoord;
	int							TexCoordMappingMode;			
	int							TexCoordReferenceMode;
	int							TexCoordSize;
	FbxGeometryElementNormal	*m_Normal;
	int							NormalMappingMode;			
	int							NormalReferenceMode;
	int							NormalSize;
	FbxGeometryElementBinormal	*m_Binormal;
	int							BinormalMappingMode;			
	int							BinormalReferenceMode;
	int							BinormalSize;
	FbxGeometryElementTangent	*m_Tangent;
	int							TangentMappingMode;			
	int							TangentReferenceMode;
	int							TangentSize;

	for (int it= 0, itE= m_ComponentNumber; it<itE; ++it)				// foreach in the array
	{
		vec3 Center;
		m_Components.push_back				(m_ComponentName);
		m_Components.back().Number=			it;
		m_Components.back().Center=			Center;

		SVector3 Vert, Norm, Bnor, Tang;
		SVector2 TexC;
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

		#pragma region Animacje
		const int AnimStackCount = m_Scene->GetSrcObjectCount(FBX_TYPE(FbxAnimStack));
		for (int i = 0; i < AnimStackCount; i++)
		{
			FbxAnimStack* AnimStack = FbxCast<FbxAnimStack>(m_Scene->GetSrcObject(FBX_TYPE(FbxAnimStack), i));
			// AnimLayers count should be greater than 1 when using animation blending
			const int AnimLayers = AnimStack->GetMemberCount(FBX_TYPE(FbxAnimLayer));

			for (int l = 0; l < AnimLayers; l++)
			{
				FbxAnimLayer* AnimLayer = AnimStack->GetMember(FBX_TYPE(FbxAnimLayer), l);
				const int AnimCurveNodeCount = AnimLayer->GetMemberCount(FBX_TYPE(FbxAnimCurveNode));
				for (int c = 0; c < AnimCurveNodeCount; ++c)
				{
					FbxAnimCurveNode* AnimCurveNode = AnimLayer->GetMember(FBX_TYPE(FbxAnimCurveNode), c);
					const int ChannelsCount = AnimCurveNode->GetChannelsCount();
					for (int ch = 0; ch < ChannelsCount; ++ch)
					{
						FbxString ChannelName = AnimCurveNode->GetChannelName(ch);
						string ChName (ChannelName);
						const int AnimCurveCount = AnimCurveNode->GetCurveCount(ch);
						for (int c = 0; c < AnimCurveCount; ++c)
						{
							FbxAnimCurve* AnimCurve = AnimCurveNode->GetCurve(ch, c);
						}
					}
				}
			}
		}

		const bool lHasVertexCache = m_Mesh->GetDeformerCount(FbxDeformer::eVertexCache) &&
        (static_cast<FbxVertexCacheDeformer*>(m_Mesh->GetDeformer(0, FbxDeformer::eVertexCache)))->IsActive();
		const bool lHasShape = m_Mesh->GetShapeCount() > 0;
		const bool lHasSkin = m_Mesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
		const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;
		/*if (HasVertexCache)
		{
			FbxVertexCacheDeformer* VCDeformer;
			VCDeformer = static_cast<FbxVertexCacheDeformer*>(m_Mesh->GetDeformer(0, FbxDeformer::eVertexCache));
			FbxCache* Cache = VCDeformer->GetCache();

			bool ReadStatus;
			// pytanie - Czy chcemy ladowac calosc od razu? zakladam, ze tak
			ReadStatus = Cache->Read((unsigned int)pTime.GetFrameCount(), lReadBuf, lVertexCount);
		}*
		#pragma endregion

		ControlPointArray	= m_Geometry->GetControlPoints();			// get size and pointer to an array containing all the CP within a geometry
		ControlPointSize	= m_Geometry->GetControlPointsCount();

		m_TexCoord			= m_Geometry->GetElementUV();
		TexCoordSize		= m_Geometry->GetElementUVCount();
		m_Normal			= m_Geometry->GetElementNormal();
		NormalSize			= m_Geometry->GetElementNormalCount();
		m_Normal			= m_Geometry->GetElementNormal();
		NormalSize			= m_Geometry->GetElementNormalCount();
		m_Binormal			= m_Geometry->GetElementBinormal();
		BinormalSize		= m_Geometry->GetElementBinormalCount();
		m_Tangent			= m_Geometry->GetElementTangent();
		TangentSize			= m_Geometry->GetElementTangentCount();

		// Ustalanie Trybow
		if (m_TexCoord != NULL)											// Check TexCoord Mapping && Reference Modes
		{
			TexCoordMappingMode = m_TexCoord->GetMappingMode();
			TexCoordReferenceMode = m_TexCoord->GetReferenceMode();

			if (TexCoordMappingMode == KFbxLayerElement::eByControlPoint ||
				TexCoordMappingMode == KFbxLayerElement::eByPolygonVertex)
			{
				
			}
			else /*Invalid TexCoord Mapping Mode* return false;
		}
		if (m_Normal != NULL)											// Check Normal Mapping && Reference Modes
		{
			NormalMappingMode = m_Normal->GetMappingMode();
			NormalReferenceMode = m_Normal->GetReferenceMode();

			if (NormalMappingMode == KFbxLayerElement::eByControlPoint ||
				NormalMappingMode == KFbxLayerElement::eByPolygonVertex)
			{
			}
			else /*Invalid Normal Mapping Mode* return false;
		}
		if (m_Binormal != NULL)											// Check Binormal Mapping && Reference Modes
		{
			BinormalMappingMode = m_Binormal->GetMappingMode();
			BinormalReferenceMode = m_Binormal->GetReferenceMode();

			if (BinormalMappingMode == KFbxLayerElement::eByControlPoint ||
				NormalMappingMode == KFbxLayerElement::eByPolygonVertex)
			{
			}
			else /*Invalid Binormal Mapping Mode* return false;
		}
		if (m_Tangent != NULL)											// Check Tangent Mapping && Reference Modes
		{
			TangentMappingMode = m_Tangent->GetMappingMode();
			TangentReferenceMode = m_Tangent->GetReferenceMode();

			if (TangentMappingMode == KFbxLayerElement::eByControlPoint ||
				NormalMappingMode == KFbxLayerElement::eByPolygonVertex)
			{
			}
			else /*Invalid Tangent Mapping Mode* return false;
		}

		#pragma region Loading of polygons
		m_Components.back().Groups.push_back(SGroup("default"));

		int PolygonCount = m_Mesh->GetPolygonCount();
		for (int polygon = 0; polygon < PolygonCount; ++polygon)
		{
			SFace Face;
			int lPolygonSize = m_Mesh->GetPolygonSize(polygon);
			if (lPolygonSize == 3) // accept only triangles
			{
				for (int j = 0; j < lPolygonSize; j++)
				{
					int lControlPointIndex = m_Mesh->GetPolygonVertex(polygon, j);
					Face.NormIndices[j] = Face.TexCIndices[j] = Face.VertIndices[j] = lControlPointIndex;				}
			}
			m_Components.back().Groups.back().Faces.push_back(Face);
		}
		#pragma endregion

		for (int itCP= 0, itCPE= ControlPointSize; itCP<itCPE; ++itCP)	// foreach Control Point
		{
			// Wierzcholki
			Vert.X= ControlPointArray[itCP][0];
			Vert.Y= ControlPointArray[itCP][1];
			Vert.Z= ControlPointArray[itCP][2];

			#pragma region Vertex Attribs
			// TexCoordy (UVs)
			if		(TexCoordReferenceMode == KFbxLayerElement::eDirect)
			{
				TexC.U= m_TexCoord->GetDirectArray()[itCP][0];
				TexC.V= m_TexCoord->GetDirectArray()[itCP][1];
			}
			else if (TexCoordReferenceMode == KFbxLayerElement::eIndexToDirect)			// UWAGA, nie jestsmy wcale pewni, czy to dziala ! ! !
			{
				Norm.X= m_TexCoord->GetDirectArray()[m_TexCoord->GetIndexArray().GetAt(itCP)][0];
				Norm.Y= m_TexCoord->GetDirectArray()[m_TexCoord->GetIndexArray().GetAt(itCP)][1];
			}
			// Normalne
			if		(NormalReferenceMode == KFbxLayerElement::eDirect)
			{
				Norm.X= m_Normal->GetDirectArray()[itCP][0];
				Norm.Y= m_Normal->GetDirectArray()[itCP][1];
				Norm.Z= m_Normal->GetDirectArray()[itCP][2];
			}
			else if (NormalReferenceMode == KFbxLayerElement::eIndexToDirect)
			{
				Norm.X= m_Normal->GetDirectArray()[m_Normal->GetIndexArray().GetAt(itCP)][0];
				Norm.Y= m_Normal->GetDirectArray()[m_Normal->GetIndexArray().GetAt(itCP)][1];
				Norm.Z= m_Normal->GetDirectArray()[m_Normal->GetIndexArray().GetAt(itCP)][2];
			}
			// Binormalne
			if (m_Binormal != NULL)
			{
				if (BinormalReferenceMode == KFbxLayerElement::eDirect)
				{
					Bnor.X= m_Binormal->GetDirectArray()[itCP][0];
					Bnor.Y= m_Binormal->GetDirectArray()[itCP][1];
					Bnor.Z= m_Binormal->GetDirectArray()[itCP][2];
				}
				else if (BinormalReferenceMode == KFbxLayerElement::eIndexToDirect)
				{
					Bnor.X= m_Binormal->GetDirectArray()[m_Binormal->GetIndexArray().GetAt(itCP)][0];
					Bnor.Y= m_Binormal->GetDirectArray()[m_Binormal->GetIndexArray().GetAt(itCP)][1];
					Bnor.Z= m_Binormal->GetDirectArray()[m_Binormal->GetIndexArray().GetAt(itCP)][2];
				}
			}
			// Tangenty
			if (m_Tangent != NULL)
			{
				if (TangentReferenceMode == KFbxLayerElement::eDirect)
				{
					Tang.X= m_Tangent->GetDirectArray()[itCP][0];
					Tang.Y= m_Tangent->GetDirectArray()[itCP][1];
					Tang.Z= m_Tangent->GetDirectArray()[itCP][2];
				}
				else if (TangentReferenceMode == KFbxLayerElement::eIndexToDirect)
				{
					Tang.X= m_Tangent->GetDirectArray()[m_Tangent->GetIndexArray().GetAt(itCP)][0];
					Tang.Y= m_Tangent->GetDirectArray()[m_Tangent->GetIndexArray().GetAt(itCP)][1];
					Tang.Z= m_Tangent->GetDirectArray()[m_Tangent->GetIndexArray().GetAt(itCP)][2];
				}
			}
			#pragma endregion

			m_Vertices.push_back(Vert);
			m_TexCoords.push_back(TexC);
			m_Normals.push_back(Norm);
			//m_BiNormals.push_back(Bnor);
			//m_Tangents.push_back(Tang);
		}
	}

	// Wczytywanie Materialow
	SMaterial				TempMat;
	string					m_MaterialName;
	FbxSurfaceMaterial		*m_Material;
	FbxColor				PropertyColor;
	for (int it= 0, itE= m_Scene->GetMaterialCount(); it<itE; ++it)
	{
		
		m_Material= m_Scene->GetMaterial(it);
		// multiple GetNames, change if neccessary
		m_MaterialName= m_Material->GetNameOnly();
		TempMat= SMaterial();
		
		PropertyColor = m_Material->FindProperty("AmbientColor").Get<KFbxColor>();
		TempMat.Ambient[0]= PropertyColor.mRed;
		TempMat.Ambient[1]= PropertyColor.mGreen;
		TempMat.Ambient[2]= PropertyColor.mBlue;
		TempMat.Ambient[3]= PropertyColor.mAlpha;		// not sure here which one
		//TempMat.Ambient[3]= m_Material->FindProperty("AmbientFactor").Get<double>();

		PropertyColor = m_Material->FindProperty("DiffuseColor").Get<KFbxColor>();
		TempMat.Diffuse[0]= PropertyColor.mRed;
		TempMat.Diffuse[1]= PropertyColor.mGreen;
		TempMat.Diffuse[2]= PropertyColor.mBlue;
		TempMat.Diffuse[3]= PropertyColor.mAlpha;		// not sure here which one
		//TempMat.Diffuse[3]= m_Material->FindProperty("DiffuseFactor").Get<double>();

		PropertyColor = m_Material->FindProperty("SpecularColor").Get<KFbxColor>();
		TempMat.Specular[0]= PropertyColor.mRed;
		TempMat.Specular[1]= PropertyColor.mGreen;
		TempMat.Specular[2]= PropertyColor.mBlue;
		TempMat.Specular[3]= PropertyColor.mAlpha;		// not sure here which one
		//TempMat.Specular[3]= m_Material->FindProperty("SpecularFactor").Get<double>();

		//TempMat.Textures								// not used?

		if (m_Materials.find(m_MaterialName)==m_Materials.end())				// if Material not loaded, load	// not sure if not bottleneck
			m_Materials.insert(make_pair(m_MaterialName,TempMat));
	}

	m_Importer->Destroy();												// destroy the importer.
	while(0);	
	/*
	FbxManager * m_SdkManager;
    FbxScene * m_Scene;
    FbxImporter * m_Importer;
    FbxAnimLayer * m_CurrentAnimLayer;
    FbxNode * m_SelectedNode;

	 if (mImporter->Import(mScene) == true)
	 {
		 // Convert Unit System to what is used in this example, if needed
		 FbxSystemUnit SceneSystemUnit = mScene->GetGlobalSettings().GetSystemUnit();
		 if( SceneSystemUnit.GetScaleFactor() != 1.0 )
		 {
			 //The unit in this example is centimeter.
			 FbxSystemUnit::cm.ConvertScene( mScene);
		 }
		 // Convert mesh, NURBS and patch into triangle mesh
		 TriangulateRecursive(mScene->GetRootNode());
	 }
	 */
	// FIXME
	return false;
}

string CModelData::Load(ILoader const& loader)
{
	/*ifstream F (path);
	if (!F.is_open())
		return false;

	int ExtLoc = path.find_last_of('.');
	string Ext = path.substr(ExtLoc+1);

	if (Ext == "fbx")
		return _LoadFromFBX(path);
	else if (Ext == "obj")
		return _LoadFromObj(F);
	else 
		return false;*/
	// FIXME
	return string();
}

CModelData::CModelData(void)
{
}


CModelData::~CModelData(void)
{
}
