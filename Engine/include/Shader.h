#pragma once
#include "resource.h"
#include <map>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <ostream>

namespace engine {

class CVector3;
class CMatrix4;
class CColor;

namespace {

void glShaderSource_engine (GLuint shader, std::string const& shaderSource);
void glShaderSource_engine (GLuint shader, std::vector<char> const& shaderSource);

}

using std::string;

template<typename T>
class Shader :
	public CResource
{
	GLuint m_id;
	std::vector<char> m_source;

public:
	GLuint getId () const { return m_id; }

	std::string Load (ILoader const& loadParams)
	{
		auto rawData = loadParams.GetRawData("frag");
		if (!rawData)
			return string("Error in getting fragment shader data");
		m_source.clear();
		std::copy(rawData.get().begin(), rawData.get().end(), std::back_inserter(m_source));
		m_source.push_back(0);

		m_id = glCreateShader(T::_getType());
	}
	Shader(ILoader const& loader) { Load(loader); }
	
	std::string Shader::Status() const
	{
		GLint compiled;

		glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
		if (compiled != GL_TRUE)
			return T::_getName() + " compilation error : " + _getInfo(m_id);
	}
	std::string Compile ()
	{
		glShaderSource_engine(m_id, m_source);
		glCompileShader(m_id);
	}
	void Unload ()
	{
		glDeleteShader(m_id);
	}	

	~Shader () { Unload(); }

	explicit operator bool()
	{
		// if the returned string is empty, no errors occured.
		return Validate().empty();
	}

protected:
	Shader() { }
};

class VertexShader : public Shader<VertexShader>
{
	static const std::string _getName() { return "Vertex Shader"; }
	static const GLint _getType() { return GL_VERTEX_SHADER; }
public:
	VertexShader() : Shader() { }
	VertexShader(ILoader const& loader) : Shader(loader) { }
};

class FragmentShader : public Shader<FragmentShader>
{
	static const std::string _getName() { return "Fragment Shader"; }
	static const GLint _getType() { return GL_FRAGMENT_SHADER; }
public:
	FragmentShader() : Shader() { }
	FragmentShader(ILoader const& loader) : Shader(loader) { }
};

class Program
{
	unsigned m_id;

	std::string _getInfo(unsigned num);
	std::map<std::string, int> m_vertexAttribs;
	std::map<std::string, int> m_uniformCache;

	std::shared_ptr<VertexShader> m_vertexShader;
	std::shared_ptr<FragmentShader> m_fragmentShader;
	
public:
	void Unload ();

	void Bind();
	void SetUniform (const std::string& name, float a);
	void SetTex (const std::string& name, unsigned texUnitNum);
	void SetUniform (std::string const& name, glm::vec2 const& vec);
	void SetUniform (std::string const& name, glm::vec3 const& vec);
	void SetUniform (std::string const& name, glm::vec4 const& color);
	void SetUniform (std::string const& name, glm::mat4 const& mat);

	int GetUniformLocation (const std::string& name);
	int GetProgramNum () const { return m_id; }
	int GetAttribLocation (const std::string& name);
	void BindAttribLocation (const std::string& name, int location);

	void AttachShader(std::shared_ptr<VertexShader> shader) {
		m_vertexShader = std::move(shader);
		glAttachShader(m_id, m_vertexShader->getId());
	}
	void AttachShader(std::shared_ptr<FragmentShader> shader) {
		m_fragmentShader = std::move(shader);
		glAttachShader(m_id, m_fragmentShader->getId());
	}

	static void DisableAll();

	void DebugDump (std::ostream&);

	bool Validate () const;
	std::string Link();
	explicit operator bool();
};

namespace {

void glShaderSource_engine (GLuint shader, std::string const& shaderSource)
{
	if (shaderSource.empty())
		throw std::exception("Empty shader passed to `glShaderSource`");
	const GLchar* ptr = shaderSource.c_str();
	const GLint size = shaderSource.size();
	glShaderSource(shader, 1, &ptr, &size);
}

void glShaderSource_engine (GLuint shader, std::vector<char> const& shaderSource)
{
	if (shaderSource.empty())
		throw std::exception("Empty shader passed to `glShaderSource`");
	const GLchar* ptr = &shaderSource[0];
	const GLint size = shaderSource.size();
	glShaderSource(shader, 1, &ptr, &size);
}

} // anonymous namespace

} // namespace engine