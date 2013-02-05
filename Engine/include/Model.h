#pragma once
#include <string>
#include <map>
#include <vector>
#include "Resource.h"
#include "IModelCooker.h"

class CModelData;
class CShader;

class CModel :
	public CResource
{
protected:
	IModelCooker* m_Cooker;

	unsigned int m_DefaultGroup;
	float m_Scale;

	bool _LoadFromCollada (const std::string& filename);

public:
	CShader* Shader;

	std::string CModel::Load(ILoader const& loadParams);
	void Unload() { }

	inline void SetScale (float scale) { m_Scale = scale; }
	void Draw () const;
	void Draw (const glm::vec3 const& pos, glm::vec3 const& rot, glm::vec3 const& scale) const;

	CModel(void);
	~CModel(void);
};

