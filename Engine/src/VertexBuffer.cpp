#include "VertexBuffer.h"
#include <GL/glew.h>

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

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

void CVertexBuffer::LoadVertices(unsigned int number_of_vertices, CVertex* data, unsigned int* indices)
{
	//za argument przyjmuje wskaznik na tablice struktur przedstawiajacych wierzcholek w formacie
	//GLfloat x, y, z
	//GLfloat nx, ny, nz
	//GLfloat s0, t0         //Texcoord0
	//GLfloat s1, t1         //Texcoord1
	//GLfloat s2, t2         //Texcoord2
	//dodatkowo wyrownane do 64bajtow czyli na koncu jakies smiec 16 bajtowy trzeba dodac
	//nie jestem pewien czy to konieczne, ale zawsze lepiej sluchac specjalistow:P
	_GuardedBind();

	GLsizei size = sizeof(data);

	glBufferData(m_Type, number_of_vertices*size, data, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, size, BUFFER_OFFSET(0));
	glNormalPointer(GL_FLOAT, size, BUFFER_OFFSET(12));
	glClientActiveTexture(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);    //Notice that after we call glClientActiveTexture, we enable the array
	glTexCoordPointer(2, GL_FLOAT, size, BUFFER_OFFSET(24));
	glClientActiveTexture(GL_TEXTURE1);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);    //Notice that after we call glClientActiveTexture, we enable the array
	glTexCoordPointer(2, GL_FLOAT, size, BUFFER_OFFSET(32));
	glClientActiveTexture(GL_TEXTURE2);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);    //Notice that after we call glClientActiveTexture, we enable the array
	glTexCoordPointer(2, GL_FLOAT, size, BUFFER_OFFSET(40));

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

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