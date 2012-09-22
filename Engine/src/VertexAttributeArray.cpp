#include "StdAfx.h"
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

CVertexAttributeArray::CVertexAttributeArray(void) :
	m_Id(0)
{
}


CVertexAttributeArray::~CVertexAttributeArray(void)
{
	glDeleteVertexArrays(1, &m_Id);
}
