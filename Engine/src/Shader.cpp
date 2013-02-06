#include <GL/glew.h>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "ResManager.h"
#include "Image.h"
#include "Misc.h"
using namespace std;

using glm::vec3;
using glm::vec4;
using glm::mat4;

template<class T> T& as_lvalue(T&& v){ return v; }

std::string CShader::Load(ILoader const& loadParams)
{
	//FIXME
	auto Frag = loadParams.GetRawData("frag");
	if (!Frag)
		return string("Error in getting fragment shader data");

	auto Vert = loadParams.GetRawData("vert");
	if (!Vert)
		return string("Error in gettin vertex shader data");

	m_FragFileData.clear();
	m_VertFileData.clear();
	std::copy(Frag.get().begin(), Frag.get().end(), std::back_inserter(m_FragFileData));
	std::copy(Vert.get().begin(), Vert.get().end(), std::back_inserter(m_VertFileData));

	m_FragNum = glCreateShader(GL_FRAGMENT_SHADER);		
	m_VertNum = glCreateShader(GL_VERTEX_SHADER);

	m_ProgramNum = glCreateProgram();

	m_Attribs["in_Position"] = 0;
	m_Attribs["in_Normal"] = 1;
	m_Attribs["in_TexCoord"] = 2;

	string CompilationResult = Compile();

	return CompilationResult;
}

void CShader::Unload() 
{ 
	glDeleteShader(m_FragNum);
	glDeleteShader(m_VertNum);
	glDeleteProgram(m_ProgramNum);
}

string CShader::Compile()
{
	glShaderSource(m_FragNum, 1, static_cast<const GLchar**>(&as_lvalue(m_FragFileData.data())), NULL);
	glCompileShader(m_FragNum);

	GLint compiled;

	glGetObjectParameterivARB(m_FragNum, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
		return "Fragment shader compilation error : " + _GetInfo(m_FragNum);;

	glShaderSource(m_VertNum, 1, static_cast<const GLchar**>(&as_lvalue(m_VertFileData.data())), NULL);
	glCompileShader(m_VertNum);

	glGetObjectParameterivARB(m_VertNum, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
		return "Vertex shader compilation error : " + _GetInfo(m_VertNum);;

	// After recompilations, attributes need to be rebound
	for (auto it = m_Attribs.begin(); it != m_Attribs.end(); ++it)
	{
		glBindAttribLocation(m_ProgramNum, it->second, it->first.c_str());
	}

	// Bind the output
	glBindFragDataLocation(m_ProgramNum, 0, "out_FragColor");

	unsigned int Shaders[4];
	GLsizei Count;

	// We don't want to bind the shaders twice, if it's just recompilation
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
		return "Program link error : " + _GetInfo(m_ProgramNum);;

	// Success
	return string();
}

bool CShader::Validate()
{
	int isValid;
	glValidateProgram(m_ProgramNum);
	glGetProgramiv(m_ProgramNum, GL_VALIDATE_STATUS, &isValid);
	return (isValid == GL_TRUE);
}


int CShader::GetAttribLocation (const std::string& name)
{
	return glGetAttribLocation(m_ProgramNum, name.c_str());
}

string CShader::_GetInfo(unsigned num)
{
	GLint blen = 0;	
	GLsizei slen = 0;

	glGetShaderiv(num, GL_INFO_LOG_LENGTH , &blen);       

	if (blen > 0)
	{
		char* compiler_log = new char [blen];

		glGetInfoLogARB(num, blen, &slen, compiler_log);

		string CompilerLogStr (compiler_log);
		delete[] compiler_log;

		return CompilerLogStr;
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

void CShader::SetUniformVector3 (std::string const& name, vec3 const& vec)
{
	GLint Loc = GetUniformLocation(name);
	if (Loc != -1)
	{
		Bind();
		glUniform3fv (Loc, 1, reinterpret_cast<const float*>(&vec));
	}
}

void CShader::SetUniformMatrix4 (std::string const& name, mat4 const& mat)
{
	GLint Loc = GetUniformLocation(name);
	if (Loc != -1)
	{
		Bind();
		glUniformMatrix4fv(Loc, 1, GL_FALSE, glm::value_ptr(mat));
	}
}

void CShader::SetUniformColor (std::string const& name, vec4 const& color)
{
	GLint Loc = GetUniformLocation(name);
	if (Loc != -1)
	{
		Bind();
		glUniform4fv (Loc, 1, glm::value_ptr(color));
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

void CShader::SetTex (const std::string& name, unsigned texUnitNum)
{
	int my_sampler_uniform_location = GetUniformLocation(name);

	glUseProgram(m_ProgramNum);
	glUniform1i(my_sampler_uniform_location, texUnitNum);
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