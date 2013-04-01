#pragma once
#include <utility>
#include "gl_id.h"

namespace engine {

class VertexAttributeArray
{
private:
    gl_id m_Id;

public:
    void Bind();
    void Draw(int startIx, int endIx);
    void EnableAttributeArray(unsigned index);

    static unsigned GetMaxVertexAttributes();

    VertexAttributeArray();
    VertexAttributeArray(VertexAttributeArray&& other) :
        m_Id(std::move(other.m_Id))
    { }
    ~VertexAttributeArray(void);
};

} // namespace engine