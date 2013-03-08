#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "ResManager.h"
#include "image.h"
#include "Misc.h"

namespace engine {

using namespace std;

using glm::vec3;
using glm::vec2;
using glm::vec4;
using glm::mat4;

namespace {

template<class T> T& as_lvalue(T&& v){ return v; }

}

std::string Program::_getInfo(unsigned num)
	{
		GLint blen = 0;	
		GLsizei slen = 0;

		glGetShaderiv(num, GL_INFO_LOG_LENGTH , &blen);       

		if (blen > 0)
		{
			char* compiler_log = new char [blen];

			glGetInfoLogARB(num, blen, &slen, compiler_log);

			std::string CompilerLogStr (compiler_log);
			delete[] compiler_log;

			return CompilerLogStr;
		}
		return std::string("No error message");
	}

std::string Program::Link()
{
	_generateId();
	// After recompilations, attributes need to be rebound
	for (auto const& attrib : m_vertexAttribs)
	{
		glBindAttribLocation(m_id, attrib.second, attrib.first.c_str());
	}

	// Bind the output
	glBindFragDataLocation(m_id, 0, "out_FragColor");

	unsigned int Shaders[4];
	GLsizei Count;

	// We don't want to bind the shaders twice, if it's just recompilation
	glGetAttachedShaders(m_id, 4, &Count, Shaders); 

/*	if (m_vertexShader)
		glAttachShader(m_id, m_vertexShader->getId());*/
	glLinkProgram (m_id);

	GLint linked;
	glGetProgramiv(m_id, GL_LINK_STATUS, &linked);
	if (linked != GL_TRUE)
		return "Program link error : " + _getInfo(m_id);;

	// Success
	return string();
}

bool Program::Validate()
{
	_generateId();
	int isValid;
	glValidateProgram(m_id);
	glGetProgramiv(m_id, GL_VALIDATE_STATUS, &isValid);
	return (isValid == GL_TRUE);
}


int Program::GetAttribLocation (const std::string& name)
{
	return glGetAttribLocation(m_id, name.c_str());
}

void Program::Bind()
{
	_generateId();
	glUseProgram(m_id);
	//glEnable(GL_FRAGMENT_PROGRAM_ARB);
}

void Program::SetUniform(const string& name, float a)
{
	GLint Loc = GetUniformLocation(name);
	if (Loc != -1)
	{
		Bind();
		glUniform1fv(Loc, 1, &a);
	}
}

void Program::SetUniform (std::string const& name, vec2 const& vec)
{
	GLint Loc = GetUniformLocation(name);
	if (Loc != -1)
	{
		Bind();
		glUniform2fv (Loc, 1, glm::value_ptr(vec));
	}
}

void Program::SetUniform (std::string const& name, vec3 const& vec)
{
	GLint Loc = GetUniformLocation(name);
	if (Loc != -1)
	{
		Bind();
		glUniform3fv (Loc, 1, glm::value_ptr(vec));
	}
}

void Program::SetUniform (std::string const& name, mat4 const& mat)
{
	GLint Loc = GetUniformLocation(name);
	if (Loc != -1)
	{
		Bind();
		glUniformMatrix4fv(Loc, 1, GL_FALSE, glm::value_ptr(mat));
	}
}

void Program::SetUniform (std::string const& name, vec4 const& color)
{
	GLint Loc = GetUniformLocation(name);
	if (Loc != -1)
	{
		Bind();
		glUniform4fv (Loc, 1, glm::value_ptr(color));
	}
}

int Program::GetUniformLocation (const string& name)
{
	auto Iter = m_uniformCache.find(name);
	if (Iter != m_uniformCache.end())
		return Iter->second;
	else
	{
		_generateId();
		int Location = glGetUniformLocation(m_id, name.c_str());
		m_uniformCache.insert(std::make_pair(name, Location));
		return Location;
	}
}

void Program::SetTex (const std::string& name, unsigned texUnitNum)
{
	int my_sampler_uniform_location = GetUniformLocation(name);

	Bind();
	glUniform1i(my_sampler_uniform_location, texUnitNum);
}

void Program::DisableAll ()
{
	glUseProgram (0);
}

void Program::BindAttribLocation (std::string const& name, int location)
{
	m_vertexAttribs[name] = location;
}

void Program::DebugDump(std::ostream& Out)
{
	int count;
	const int MAX_NAME_SIZE = 30;
	char Name[MAX_NAME_SIZE];
	int Len, Size;
	GLenum Type;

	glGetObjectParameterivARB (m_id, GL_OBJECT_ACTIVE_ATTRIBUTES_ARB, &count);
	Out << "Attributes Count = " << count << endl;
	for (int i = 0; i < count; ++i)
	{
		glGetActiveAttrib (m_id, i, MAX_NAME_SIZE, &Len, &Size, &Type, Name);
		Out << i << ". " << Name << " " << Type << "(" << Size << ")\n"; 
	}

	Out << endl;
	
	glGetObjectParameterivARB (m_id, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &count);
	Out << "Uniforms Count = " << count << endl;
	for (int i = 0; i < count; ++i)
	{		
		glGetActiveUniform (m_id, i, MAX_NAME_SIZE, &Len, &Size, &Type, Name);
		Out << i << ". " << Name << " " << Type << "(" << Size << ")\n"; 
	}
}

Program::operator bool()
{
	return Validate();
}

} // namespace engine