#pragma once
#include <string>
#include <set>
#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include "ShaderSet.h"
#include <functional>
#include <string>

class CModel;

class CScene
{
protected:
	struct SSceneObject
	{
		CModel* Model;
		glm::vec3 Position, Rotation, Scale;
		CShader* Shader;
		std::vector<std::string> Textures;

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

