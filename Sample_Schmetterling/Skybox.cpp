#pragma once
#include <singleton.h>
#include <Image.h>
#include <Shader.h>
#include <VertexBuffer.h>
#include <VertexAttributeArray.h>
#include <vector>	
#include <Vector2.h>
#include <Vector3.h>
#include "Skybox.h"

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

void CSkybox::Draw()
{
	//glEnable(GL_VERTEX_ARRAY);
	m_Vao.Bind();
	m_Shader->Bind();
	for(int it=0, itE=m_Texture.size(); it!=itE; ++it)
	{
		m_Texture[it]->Bind(it);
		m_Shader->SetTex("Texture"+boost::lexical_cast<string>(it),it);
	}

	//glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glDrawArrays(GL_QUADS, 0, 5*4);
}

void CSkybox::Move(float x, float y, float z)
{
	m_X= x;
	m_Y= y;
	m_Z= z;
}
void CSkybox::Resize(float Width, float Height)
{
	m_Width= Width;
	m_Height= Height;
}
CSkybox* CSkybox::Init(float WSize, float HSize)
{
	m_X= 0 - WSize/2;
	m_Y= 0 + HSize*1.5;
	m_Z= 0 - WSize/2;

	m_Width = 2 * WSize;
	m_Height = 2 * HSize;

	std::vector<unsigned int> Indices;
	std::vector<CVector3> Vertices;
	std::vector<CVector2<float>> UVs;
	
	static float Verts[] =
	{
		1,0,0,	1,0,1,	1,1,1,	1,1,0, //1
		1,0,0,	1,1,0,	0,1,0,	0,0,0, //2
		1,1,0,	1,1,1,	0,1,1,	0,1,0,
		1,1,1,	1,0,1,	0,0,1,	0,1,1,
		0,1,0,	0,1,1,	0,0,1,	0,0,0
	};
	static float TexCoords[] = {
			
		0,1,	0,0,	1,0,	1,1,		// 1
		1,0,	2,0,	2,1,	1,1,		// 2
		3,1,	3,2,	2,2,	2,1,		//top
		2,3,	1,3,	1,2,	2,2,		// 4
		2,1,	2,2,	1,2,	1,1			// 3
	};

	for (int i = 0; i < 4*5; ++i)
	{
		Verts[i*3+0] -= 0.5f;
		Verts[i*3+1] -= 0.5f;
		Verts[i*3+2] -= 0.5f;

		Verts[i*3+0] *= 1600;
		Verts[i*3+1] *= 400;
		Verts[i*3+2] *= 1600;
	}

	for (int i = 0; i < 2*4*5; ++i)
		TexCoords[i] *= (1.f/3.f);

	m_Vao.Bind();

	//m_IndicesVbo.Bind();
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size()*sizeof(unsigned int), &(Indices[0]), GL_STATIC_DRAW);

	m_VerticesVbo.Bind();
	glBufferData(GL_ARRAY_BUFFER, 3*4*5*sizeof(float), Verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	m_UVsVbo.Bind();
	glBufferData(GL_ARRAY_BUFFER, 2*4*5*sizeof(float), TexCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, Vertices.size());
	
	//glDrawElements(GL_TRIANGLE_STRIP, HSize.X * 2, GL_UNSIGNED_INT, BUFFER_OFFSET((i * HSize.X * 2) * sizeof(unsigned int)));
	return this;
}