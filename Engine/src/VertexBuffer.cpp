#include "VertexBuffer.h"
#include <GL/glew.h>

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

namespace engine {

void CVertexBuffer::_UpdateCachedValue()
{
	switch (m_Type)
	{
	case DATA_BUFFER:
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &m_savedVBOId);
		break;
	case INDEX_BUFFER:
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &m_savedVBOId);
		break;
	}
}

void CVertexBuffer::_GenerateVBOId()
{
	glGenBuffers(1, &m_VBOId);
}

void CVertexBuffer::_GuardedBind()
{
	_UpdateCachedValue();
	if (m_VBOId == 0)
		_GenerateVBOId();

	if(m_VBOId != (GLuint)m_savedVBOId)
		glBindBuffer(m_Type, m_VBOId);
}

void CVertexBuffer::_GuardedUnbind()
{
	if(m_VBOId != (GLuint)m_savedVBOId)
		glBindBuffer(m_Type, (GLuint)m_savedVBOId);
}

void CVertexBuffer::Bind()
{
	if (m_VBOId == 0)
		_GenerateVBOId();

	glBindBuffer(m_Type, m_VBOId);
}

void CVertexBuffer::LoadData(const void* data, int size)
{
	_GuardedBind();
	glBufferData(m_Type, size, data, m_Usage);
	_GuardedUnbind();
}

CVertexBuffer::CVertexBuffer(EBufferType type, EBufferUsage usage): 
	m_Type(type),
	m_Usage(usage),
	m_VBOId(0),
	m_savedVBOId(0)
{
	_UpdateCachedValue();
}

CVertexBuffer::~CVertexBuffer()
{
	glDeleteBuffers(1, &m_VBOId);
}

} // namespace engine