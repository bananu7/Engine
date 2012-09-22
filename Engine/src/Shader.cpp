#include "stdafx.h"
#include "Matrix4.h"
#include "Shader.h"
#include <fstream>
#include "ResManager.h"
#include "Image.h"
#include "Misc.h"
using namespace std;

std::string CShader::Load(SLoadParams const& loadParams)
{
	string FragFilePath, VertFilePath;
	if (!loadParams.GetParam("frag", FragFilePath))
		return string("Missing 'frag' load param");
	if (!loadParams.GetParam("vert", VertFilePath))
		return string("Missing 'vert' load param");

	FragFilePath = CResManager::GetSingleton()->GetResourcePath() + FragFilePath;
	VertFilePath = CResManager::GetSingleton()->GetResourcePath() + VertFilePath;

	m_FragFileData = TextFileRead(FragFilePath.c_str());
	m_VertFileData = TextFileRead(VertFilePath.c_str());

	if (!m_FragFileData)
		return string("Error loading fragment shader file");
	if (!m_VertFileData)
		return string("Error loading vertex shader file");

	m_FragNum = glCreateShader(GL_FRAGMENT_SHADER);		
	m_VertNum = glCreateShader(GL_VERTEX_SHADER);

	m_ProgramNum = glCreateProgram();

	m_Attribs["in_Position"] = 0;
	m_Attribs["in_Normal"] = 1;
	m_Attribs["in_TexCoord"] = 2;

	return Compile() ? string("") : string("Compile error");
}

void CShader::Unload() 
{ 
	free (m_FragFileData);
	free (m_VertFileData);
	// TODO: destroy opengl programs and shaders
}

int CShader::GetAttribLocation (const std::string& name)
{
	return glGetAttribLocation(m_ProgramNum, name.c_str());
}

string CShader::_GetInfo(uint32 num)
{
	GLint blen = 0;	
	GLsizei slen = 0;

	glGetShaderiv(num, GL_INFO_LOG_LENGTH , &blen);       

	if (blen > 1)
	{
		char* compiler_log = (char*)malloc(blen);

		glGetInfoLogARB(num, blen, &slen, compiler_log);
		return string(compiler_log);
		free (compiler_log);
	}
	return string("No error message");
}

void CShader::Bind()
{
	glUseProgram(m_ProgramNum);
	//glEnable(GL_FRAGMENT_PROGRAM_ARB);
}

void CShader::SetUniform1f(const string& name, float a)
{
	GLint Loc = GetUniformLocation(name);
	if (Loc != -1)
	{
		Bind();
		glUniform1fv(Loc, 1, &a);
	}
}

void CShader::SetUniformVector3 (std::string const& name, CVector3 const& vec)
{
	GLint Loc = GetUniformLocation(name);
	if (Loc != -1)
	{
		Bind();
		glUniform3fv (Loc, 1, reinterpret_cast<const float*>(&vec));
	}
}

void CShader::SetUniformMatrix4 (std::string const& name, CMatrix4 const& mat)
{
	GLint Loc = GetUniformLocation(name);
	if (Loc != -1)
	{
		Bind();
		glUniformMatrix4fv(Loc, 1, GL_FALSE, mat.GetRawData());
	}
}

void CShader::SetUniformColor (std::string const& name, CColor const& color)
{
	GLint Loc = GetUniformLocation(name);
	if (Loc != -1)
	{
		Bind();
		glUniform4fv (Loc, 1, reinterpret_cast<const float*>(&color));
	}
}

int CShader::GetUniformLocation (const string& name)
{
	auto Iter = m_UniformCache.find(name);
	if (Iter != m_UniformCache.end())
		return Iter->second;
	else
	{
		int Location = glGetUniformLocation(m_ProgramNum, name.c_str());
		m_UniformCache.insert(std::make_pair(name, Location));
		return Location;
	}
}

void CShader::SetTex (const std::string& name, uint8 texUnitNum)
{
	int my_sampler_uniform_location = GetUniformLocation(name);

	glUseProgram(m_ProgramNum);
	glUniform1i(my_sampler_uniform_location, texUnitNum);
}

bool CShader::Compile()
{
	glShaderSource(m_FragNum, 1, static_cast<const GLchar**>(const_cast<const char**>(&m_FragFileData)), NULL);
	glCompileShader(m_FragNum);

	GLint compiled;

	ofstream Log ("shader_out.txt", ios::out | ios::app);

	glGetObjectParameterivARB(m_FragNum, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		string Temp = _GetInfo(m_FragNum);
		Log << "Frag : " << Temp;
		return false;
	}      

	glShaderSource(m_VertNum, 1, static_cast<const GLchar**>(const_cast<const char**>(&m_VertFileData)), NULL);
	glCompileShader(m_VertNum);

	glGetObjectParameterivARB(m_VertNum, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		string Temp = _GetInfo(m_VertNum);
		Log << "Vert: " << Temp;
		return false;
	}      

	for (auto it = m_Attribs.begin(); it != m_Attribs.end(); ++it)
	{
		glBindAttribLocation(m_ProgramNum, it->second, it->first.c_str());
	}

	// ...tudziez wyjscie
	glBindFragDataLocation(m_ProgramNum, 0, "out_FragColor");

	unsigned int Shaders[4];
	GLsizei Count;
	glGetAttachedShaders(m_ProgramNum, 4, &Count, Shaders); 

	if (Count == 0)
	{
		glAttachShader (m_ProgramNum, m_FragNum);
		glAttachShader (m_ProgramNum, m_VertNum);
	}
	glLinkProgram (m_ProgramNum);

	GLint linked;
	glGetProgramiv(m_ProgramNum, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		string Temp = _GetInfo(m_ProgramNum);
		Log << "Prog: " << Temp;
		return false;
	}     
	return true;
}

bool CShader::Validate()
{
	int isValid;
	glValidateProgram(m_ProgramNum);
	glGetProgramiv(m_ProgramNum, GL_VALIDATE_STATUS, &isValid);
	return (isValid == GL_TRUE);
}

void CShader::DisableAll ()
{
	glUseProgram (0);
}

void CShader::BindAttribLocation (std::string const& name, int location)
{
	m_Attribs[name] = location;
}

void CShader::DebugDump()
{
	int count;
	const int MAX_NAME_SIZE = 30;
	char Name[MAX_NAME_SIZE];
	int Len, Size;
	GLenum Type;
	std::ofstream Out ("shader_dump.txt");

	glGetObjectParameterivARB (m_ProgramNum, GL_OBJECT_ACTIVE_ATTRIBUTES_ARB, &count);
	Out << "Attributes Count = " << count << endl;
	for (int i = 0; i < count; ++i)
	{
		glGetActiveAttrib (m_ProgramNum, i, MAX_NAME_SIZE, &Len, &Size, &Type, Name);
		Out << i << ". " << Name << " " << Type << "(" << Size << ")\n"; 
	}

	Out << endl;
	
	glGetObjectParameterivARB (m_ProgramNum, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &count);
	Out << "Uniforms Count = " << count << endl;
	for (int i = 0; i < count; ++i)
	{		
		glGetActiveUniform (m_ProgramNum, i, MAX_NAME_SIZE, &Len, &Size, &Type, Name);
		Out << i << ". " << Name << " " << Type << "(" << Size << ")\n"; 
	}
}

CShader::CShader(void)
{
}


CShader::~CShader(void)
{
}