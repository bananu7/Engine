#pragma once
#include "Color.h"
#include <unordered_set>
#include <glm/glm.hpp>
#include <string>

class Program;
class CVector3;
class CMatrix4;

class ProgramSet
{
	std::unordered_set<Program*> m_Shaders;

public:
	void SetUniform (const std::string& name, float a);
	void SetTex (const std::string& name, unsigned texUnitNum);
	void SetUniform (std::string const& name, glm::vec3 const& vec);
	void SetUniform (std::string const& name, glm::vec4 const& color);
	void SetUniform (std::string const& name, glm::mat4 const& mat);

	void AddShader(Program* shader);

	ProgramSet(void);
	~ProgramSet(void);
};

