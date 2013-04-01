#include "VertexBuffer.h"
#include <GL/glew.h>

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

namespace engine {

void VertexBuffer::_UpdateCachedValue()
{
    int temp;
    switch (m_Type)
    {
    case DATA_BUFFER:
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &temp);
        break;
    case INDEX_BUFFER:
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &temp);
        break;
    }

    m_savedVBOId = temp;
}

void VertexBuffer::_GenerateVBOId()
{
    glGenBuffers(1, m_VBOId.get_ptr());
}

void VertexBuffer::_GuardedBind()
{
    _UpdateCachedValue();
    if (m_VBOId == 0)
        _GenerateVBOId();

    if(m_VBOId != (GLuint)m_savedVBOId)
        glBindBuffer(m_Type, m_VBOId);
}

void VertexBuffer::_GuardedUnbind()
{
    if(m_VBOId != (GLuint)m_savedVBOId)
        glBindBuffer(m_Type, (GLuint)m_savedVBOId);
}

void VertexBuffer::Bind()
{
    if (m_VBOId == 0)
        _GenerateVBOId();

    glBindBuffer(m_Type, m_VBOId);
}

void VertexBuffer::LoadData(const void* data, int size)
{
    _GuardedBind();
    glBufferData(m_Type, size, data, m_Usage);
    _GuardedUnbind();
}

VertexBuffer::VertexBuffer(EBufferType type, EBufferUsage usage): 
    m_Type(type),
    m_Usage(usage)
{
    _UpdateCachedValue();
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, m_VBOId.get_ptr());
}

} // namespace engine