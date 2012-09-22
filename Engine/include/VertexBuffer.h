#pragma once

#include "stdafx.h"

struct CVertex;

class CVertexBuffer
{
public:
	enum EBufferUsage
	{
		STATIC_DRAW = GL_STATIC_DRAW,
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW
		/*STATIC_READ,
		STATIC_WRITE,
		DYNAMIC_READ,
		DYNAMIC_WRITE*/
	};
	enum EBufferType
	{
		DATA_BUFFER = GL_ARRAY_BUFFER,
		INDEX_BUFFER = GL_ELEMENT_ARRAY_BUFFER
	};
private:
	GLuint	m_VBOId;
	GLint	m_savedVBOId;

	EBufferUsage m_Usage;
	EBufferType m_Type;

protected:
	GLuint	_GenerateVBOId();
	void	_GuardedBind();
	void	_GuardedUnbind();
public:
	void Bind();
	void LoadVertices(GLuint number_of_vertices, CVertex* data, GLuint* indices);
	void LoadData (const void* data, int size);

	CVertexBuffer(EBufferType type, EBufferUsage usage);
	~CVertexBuffer();
};