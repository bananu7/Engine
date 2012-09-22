#include "stdafx.h"
#include "Model.h"
#include "ResManager.h"
#include "Image.h"
#include "ModelCookerDisplayList.h"
#include "ModelCookerVBO.h"
#include "ModelData.h"
#include "Shader.h"
#include "Matrix4.h"
#include "Camera.h"
#include "ResManager.h"
using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

string CModel::Load(SLoadParams const& loadParams)
{
	auto const& Iter = loadParams.Params.find("path");
	if (Iter == loadParams.Params.end())
		return string("Missing 'path' load param");
	string const& Path = Iter->second;

	CModelData Data;
	if (!Data.Load(CResManager::GetSingleton()->GetResourcePath() + Path))
		return string("Error with data loading");

	//m_Cooker = new CModelCookerDisplayList();
	m_Cooker = new CModelCookerVBO();
	
	if (!m_Cooker->Cook(Data))
		return string("Error with cooking");

	return string();
}

inline void SetTransform (const std::string& groupName, const CTransform& t)
{
	// TODO: implement
	//m_Groups[groupName].Transform = t;
}

void CModel::Draw () const
{
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glScalef(m_Scale, m_Scale, m_Scale);
	m_Cooker->Draw();
	//glPopMatrix();
}

void CModel::Draw(const CVector3& pos, const CVector3& rot, const CVector3& scale) const
{
	CMatrix4 PMat = CCamera::CreateTranslation(pos.X, pos.Y, pos.Z);
	CMatrix4 RMat = CCamera::CreateRotation(rot.X, rot.Y, rot.Z);
	CMatrix4 SMat = CCamera::CreateScale(scale.X, scale.Y, scale.Z);
	PMat.Mult(RMat);
	PMat.Mult(SMat);
	Shader->SetUniformMatrix4("ModelMatrix", PMat);
	Draw();
}
CModel::CModel(void)
{
	m_Scale = 1.0f;
}


CModel::~CModel(void)
{
}
