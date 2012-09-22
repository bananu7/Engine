#pragma once
#include <string>
#include <set>
#include <vector>
#include "Vector3.h"
#include "Shader.h"
#include "ShaderSet.h"
#include <functional>

class CModelFbx;

class CScene
{
protected:
	struct SSceneObject
	{
		CModelFbx*						Model;
		CVector3						Position;
		CVector3						Rotation, Scale;
		CShader*						Shader;
		std::vector<std::string>		Textures;

		void Draw () const;
	};

	CShaderSet*				m_ShaderSet;
	std::set<SSceneObject*> m_Objects;

public:
	std::tr1::function<float(float x, float y)> m_GroundSampler;

	bool LoadFromFile (const std::string& path);
	void SetShaderSet (CShaderSet& ShaderSet) { m_ShaderSet = &ShaderSet; }
	void Draw ();

	CScene(void);
	~CScene(void);
};

