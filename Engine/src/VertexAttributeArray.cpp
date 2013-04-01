#include <GL/glew.h>
#include "VertexAttributeArray.h"

namespace engine {

void VertexAttributeArray::Bind()
{
    if (m_Id == 0)
        glGenVertexArrays(1, m_Id.get_ptr());

    glBindVertexArray(m_Id);
}

void VertexAttributeArray::Draw(int startIx, int endIx)
{
    Bind();
    glDrawArrays(GL_TRIANGLES, startIx, endIx);
}

void VertexAttributeArray::EnableAttributeArray(unsigned index)
{
    Bind();
    glEnableVertexAttribArray(index);
}

unsigned VertexAttributeArray::GetMaxVertexAttributes()
{
    GLint Count;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &Count);
    return Count;
}

VertexAttributeArray::VertexAttributeArray() { }

VertexAttributeArray::~VertexAttributeArray(void)
{
    glDeleteVertexArrays(1, m_Id.get_ptr());
}

} // namespace engine