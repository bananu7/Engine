
#include "ShaderSet.h"
#include "Shader.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

void CShaderSet::SetUniform (const std::string& name, float a)
{
	for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
		(*It)->SetUniform(name, a);
}

void CShaderSet::SetTex (const std::string& name, unsigned texUnitNum)
{
	for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
		(*It)->SetTex(name, texUnitNum);
}

void CShaderSet::SetUniform (std::string const& name, vec3 const& vec)
{
	for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
		(*It)->SetUniform(name, vec);
}

void CShaderSet::SetUniform (std::string const& name, vec4 const& color)
{
	for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
		(*It)->SetUniform(name, color);
}

void CShaderSet::SetUniform (std::string const& name, mat4 const& mat)
{
	for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
		(*It)->SetUniform(name, mat);
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
