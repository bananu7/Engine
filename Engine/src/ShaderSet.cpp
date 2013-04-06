
#include "ShaderSet.h"
#include "Shader.h"

namespace engine {

using glm::vec3;
using glm::vec4;
using glm::mat4;

void ProgramSet::SetUniform (const std::string& name, float a)
{
    for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
        (*It)->SetUniform(name, a);
}

void ProgramSet::SetTex (const std::string& name, unsigned texUnitNum)
{
    for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
        (*It)->SetTex(name, texUnitNum);
}

void ProgramSet::SetUniform (std::string const& name, vec3 const& vec)
{
    for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
        (*It)->SetUniform(name, vec);
}

void ProgramSet::SetUniform (std::string const& name, vec4 const& color)
{
    for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
        (*It)->SetUniform(name, color);
}

void ProgramSet::SetUniform (std::string const& name, mat4 const& mat)
{
    for (auto It = m_Shaders.begin(); It != m_Shaders.end(); ++It)
        (*It)->SetUniform(name, mat);
}

void ProgramSet::AddShader(Program* shader)
{
    m_Shaders.insert(shader);
}

ProgramSet::ProgramSet(void)
{
}


ProgramSet::~ProgramSet(void)
{
}

} // namespace engine