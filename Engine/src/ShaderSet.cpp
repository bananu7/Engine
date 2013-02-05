
#include "ShaderSet.h"
#include "Shader.h"

void CShaderSet::SetUniform1f (const std::string& name, float a)
{
	for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
		(*It)->SetUniform1f(name, a);
}

void CShaderSet::SetTex (const std::string& name, unsigned texUnitNum)
{
	for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
		(*It)->SetTex(name, texUnitNum);
}

void CShaderSet::SetUniformVector3 (std::string const& name, CVector3 const& vec)
{
	for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
		(*It)->SetUniformVector3(name, vec);
}

void CShaderSet::SetUniformColor (std::string const& name, CColor const& color)
{
	for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
		(*It)->SetUniformColor(name, color);
}

void CShaderSet::SetUniformMatrix4 (std::string const& name, CMatrix4 const& mat)
{
	for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
		(*It)->SetUniformMatrix4(name, mat);
}

void CShaderSet::AddShader(CShader* shader)
{
	m_Shaders.insert(shader);
}

CShaderSet::CShaderSet(void)
{
}


CShaderSet::~CShaderSet(void)
{
}
