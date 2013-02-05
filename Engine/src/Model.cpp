
#include "Model.h"
#include "ResManager.h"
#include "Image.h"
#include "ModelCookerDisplayList.h"
#include "ModelCookerVBO.h"
#include "ModelData.h"
#include "Shader.h"
#include "Camera.h"
#include "ResManager.h"
using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using glm::vec3;
using glm::mat4;

string CModel::Load(ILoader const& loadParams)
{
	// FIXME
	//auto const& Iter = loadParams.Params.find("path");
	//if (Iter == loadParams.Params.end())
	//	return string("Missing 'path' load param");
	// string const& Path = Iter->second;

	CModelData Data;
	// FIXME
	//if (!Data.Load(CResManager::GetSingleton()->GetResourcePath() + Path))
	//	return string("Error with data loading");

	//m_Cooker = new CModelCookerDisplayList();
	m_Cooker = new CModelCookerVBO();
	
	if (!m_Cooker->Cook(Data))
		return string("Error with cooking");

	return string();
}

/*inline void SetTransform (const std::string& groupName, const CTransform& t)
{
	// TODO: implement
	//m_Groups[groupName].Transform = t;
}*/

void CModel::Draw () const
{
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glScalef(m_Scale, m_Scale, m_Scale);
	m_Cooker->Draw();
	//glPopMatrix();
}

void CModel::Draw(const vec3& pos, const vec3& rot, const vec3& scale) const
{
	mat4 PMat = CCamera::CreateTranslation(pos.x, pos.y, pos.z);
	mat4 RMat = CCamera::CreateRotation(rot.x, rot.y, rot.z);
	mat4 SMat = CCamera::CreateScale(scale.x, scale.y, scale.z);
	PMat *= RMat;
	PMat *= SMat;
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
