#pragma once
#include "Color.h"
#include "Vector3.h"
#include "Matrix4.h"
#include <set>

class CShader;

class CShaderSet
{
	std::set<CShader*> m_Shaders;

public:
	void SetUniform1f (const std::string& name, float a);
	void SetTex (const std::string& name, uint8 texUnitNum);
	void SetUniformVector3 (std::string const& name, CVector3 const& vec);
	void SetUniformColor (std::string const& name, CColor const& color);
	void SetUniformMatrix4 (std::string const& name, CMatrix4 const& mat);

	void AddShader(CShader* shader);

	CShaderSet(void);
	~CShaderSet(void);
};

