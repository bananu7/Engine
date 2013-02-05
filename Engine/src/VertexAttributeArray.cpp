#include <GL/glew.h>
#include "VertexAttributeArray.h"

void CVertexAttributeArray::Bind()
{
	if (m_Id == 0)
		glGenVertexArrays(1, &m_Id);

	glBindVertexArray(m_Id);
}

void CVertexAttributeArray::Draw(int startIx, int endIx)
{
	Bind();
	glDrawArrays(GL_TRIANGLES, startIx, endIx);
}

void CVertexAttributeArray::EnableAttributeArray(unsigned index)
{
	Bind();
	glEnableVertexAttribArray(index);
}

unsigned CVertexAttributeArray::GetMaxVertexAttributes()
{
	GLint Count;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &Count);
	return Count;
}

CVertexAttributeArray::CVertexAttributeArray(void) :
	m_Id(0)
{
}

CVertexAttributeArray::~CVertexAttributeArray(void)
{
	glDeleteVertexArrays(1, &m_Id);
}
