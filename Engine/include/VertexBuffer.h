#pragma once
#include <GL/glew.h>
#include <utility>
#include "gl_id.h"

namespace engine {

class VertexBuffer
{
public:
    enum EBufferType
    {
        DATA_BUFFER = GL_ARRAY_BUFFER,
        INDEX_BUFFER = GL_ELEMENT_ARRAY_BUFFER
    };
    enum EBufferUsage
    {
        STATIC_DRAW = GL_STATIC_DRAW,
        DYNAMIC_DRAW = GL_DYNAMIC_DRAW
        /*STATIC_READ,
        STATIC_WRITE,
        DYNAMIC_READ,
        DYNAMIC_WRITE*/
    };
    
private:
    gl_id m_VBOId;
    gl_id m_savedVBOId;

    EBufferUsage m_Usage;
    EBufferType m_Type;

protected:
    void _GenerateVBOId();
    void _GuardedBind();
    void _GuardedUnbind();
    void _UpdateCachedValue();
public:
    void Bind();
    void LoadData (const void* data, int size);

    VertexBuffer(EBufferType type = EBufferType::DATA_BUFFER, EBufferUsage usage = EBufferUsage::STATIC_DRAW);
    VertexBuffer(VertexBuffer&& other) :
        m_VBOId(std::move(other.m_VBOId)),
        m_savedVBOId(std::move(other.m_savedVBOId)),
        m_Usage(other.m_Usage),
        m_Type(other.m_Type)
    { }
    ~VertexBuffer();
};

} // namespace engine