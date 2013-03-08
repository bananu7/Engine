
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

namespace engine {

using glm::vec3;
using glm::mat4;
/*string Model::Load(ILoader const& loadParams)
{
	// FIXME
	//auto const& Iter = loadParams.Params.find("path");
	//if (Iter == loadParams.Params.end())
	//	return string("Missing 'path' load param");
	// string const& Path = Iter->second;

	ModelData Data;
	// FIXME
	//if (!Data.Load(CResManager::GetSingleton()->GetResourcePath() + Path))
	//	return string("Error with data loading");

	//m_Cooker = new CModelCookerDisplayList();
	m_Cooker = new ModelCookerVBO();
	
	if (!m_Cooker->Cook(Data))
		return string("Error with cooking");

	return string();
}*/

/*inline void SetTransform (const std::string& groupName, const CTransform& t)
{
	// TODO: implement
	//m_Groups[groupName].Transform = t;
}*/

void Model::Draw () const
{
	//m_Cooker->Draw();
}

void Model::Draw(const vec3& pos, const vec3& rot, const vec3& scale) const
{
	mat4 PMat = Camera::CreateTranslation(pos.x, pos.y, pos.z);
	mat4 RMat = Camera::CreateRotation(rot.x, rot.y, rot.z);
	mat4 SMat = Camera::CreateScale(scale.x, scale.y, scale.z);
	PMat *= RMat;
	PMat *= SMat;
	Shader->SetUniform("ModelMatrix", PMat);
	Draw();
}

Model::Model(ModelData const& data)
{
}

} // namespace engine