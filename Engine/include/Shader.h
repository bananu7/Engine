#pragma once
#include "resource.h"
#include <map>
#include <glm/glm.hpp>

class CVector3;
class CMatrix4;
class CColor;

class CShader :
	public CResource
{
protected:
	std::vector<char> m_FragFileData, m_VertFileData;
	unsigned m_FragNum, m_VertNum;
	unsigned m_ProgramNum;

	std::string _GetInfo(unsigned num);
	std::map<std::string, int> m_Attribs;
	std::map<std::string, int> m_UniformCache;
	
public:
	std::string Load(ILoader const& loadParams);
	void Unload ();

	void Bind();
	void SetUniform1f (const std::string& name, float a);
	void SetTex (const std::string& name, unsigned texUnitNum);
	void SetUniformVector3 (std::string const& name, glm::vec3 const& vec);
	void SetUniformColor (std::string const& name, glm::vec4 const& color);
	void SetUniformMatrix4 (std::string const& name, glm::mat4 const& mat);

	int GetUniformLocation (const std::string& name);
	int GetProgramNum () const { return m_ProgramNum; }
	int GetAttribLocation (const std::string& name);
	void BindAttribLocation (const std::string& name, int location);

	bool Validate ();
	std::string Compile ();
	
	static void DisableAll ();
	
	void DebugDump ();

	//inline unsigned GetNum (void) const { return m_Num; };
	
	CShader(void);
	~CShader(void);
};
