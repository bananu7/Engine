#pragma once
#include <singleton.h>
#include <Image.h>
#include <ResManager.h>
#include <Shader.h>
#include <VertexBuffer.h>
#include <VertexAttributeArray.h>
#include <Matrix4.h>
#include <vector>

class CSkybox:
	public CSingleton<CSkybox>
{
protected:
	std::vector<CImage*>	m_Texture;
	Program*				m_Shader;
	CVertexBuffer			m_VerticesVbo;
	CVertexBuffer			m_NormalsVbo;
	CVertexBuffer			m_UVsVbo;
	CVertexBuffer			m_IndicesVbo;
	CVertexAttributeArray	m_Vao;
	// position of upper north western vertex
	float					m_X;
	float					m_Y;
	float					m_Z;
	// assuming it is cuboid
	float					m_Width;
	float					m_Height;

public:
	void Draw();

	void Move(float x, float y, float z);
	void Resize(float Width, float Height);
	CSkybox* Init(float WSize, float HSize);

	CSkybox():
		m_VerticesVbo(CVertexBuffer::DATA_BUFFER, CVertexBuffer::STATIC_DRAW),
		m_NormalsVbo(CVertexBuffer::DATA_BUFFER, CVertexBuffer::STATIC_DRAW),
		m_UVsVbo(CVertexBuffer::DATA_BUFFER, CVertexBuffer::STATIC_DRAW),
		m_IndicesVbo(CVertexBuffer::INDEX_BUFFER, CVertexBuffer::STATIC_DRAW)
	{
		m_Shader = CResManager::GetSingleton()->GetResource<Program>("shader_texturing");
		m_Texture.push_back(CResManager::GetSingleton()->GetResource<CImage>("skybox_tex_good"));
		m_Texture.push_back(CResManager::GetSingleton()->GetResource<CImage>("skybox_tex_evil"));
	}
};