#pragma once
#include "Resource.h"
#include "Shader.h"
#include <fbxsdk.h>

class CFbxVboMesh;

class CModelFbx :
	public CResource
{
	static FbxManager* FbxManagerInstance;
	static FbxIOSettings* FbxIOSettingsInstance;
	static FbxImporter* FbxImporterInstance;
	
	static void _InitializeFbxComponents();

protected:
	CFbxVboMesh* m_Mesh;
	FbxMesh* m_RawMesh;
	FbxScene* m_Scene;

public:
	CShader* m_Shader;

	std::string Load(ILoader const& loadParams);
	void Draw();
	void Draw (const CVector3& pos, const CVector3& rot, const CVector3& scale);

	void SetAnimationPose(int animStack, int animLayer, float time);

	void Unload() { }

	CModelFbx(void);
	~CModelFbx(void);
};

