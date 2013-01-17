#pragma once
#include "resource.h"
#include <map>

class CVector3;
class CMatrix4;
class CColor;

class CShader :
	public CResource
{
protected:
	char*	m_FragFileData, *m_VertFileData;
	uint32	m_FragNum, m_VertNum;
	uint32	m_ProgramNum;

	std::string _GetInfo(uint32 num);
	std::map<std::string, int> m_Attribs;
	std::map<std::string, int> m_UniformCache;
	
public:
	std::string Load(ILoader const& loadParams);
	void Unload ();

	void Bind();
	void SetUniform1f (const std::string& name, float a);
	void SetTex (const std::string& name, uint8 texUnitNum);
	void SetUniformVector3 (std::string const& name, CVector3 const& vec);
	void SetUniformColor (std::string const& name, CColor const& color);
	void SetUniformMatrix4 (std::string const& name, CMatrix4 const& mat);

	__declspec(deprecated("Use SetUniform* instead"))
	int GetUniformLocation (const std::string& name);

	__declspec(deprecated)
	int GetProgramNum () const { return m_ProgramNum; }

	__declspec(deprecated)
	int GetAttribLocation (const std::string& name);
	void BindAttribLocation (const std::string& name, int location);


	bool Validate ();
	std::string Compile ();
	
	static void DisableAll ();
	
	void DebugDump ();

	//inline uint32 GetNum (void) const { return m_Num; };
	
	CShader(void);
	~CShader(void);
};
