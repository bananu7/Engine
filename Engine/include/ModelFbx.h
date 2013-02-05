#pragma once
#include "Resource.h"
#include "Shader.h"
#include <fbxsdk.h>
#include <glm/glm.hpp>
#include <string>

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
	void CModelFbx::Draw (const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);

	void SetAnimationPose(int animStack, int animLayer, float time);

	void Unload() { }

	CModelFbx(void);
	~CModelFbx(void);
};

