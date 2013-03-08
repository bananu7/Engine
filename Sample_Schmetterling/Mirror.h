#pragma once
#include "Framebuffer.h"
#include "VertexBuffer.h"

class Program;

class CMirror
{
	unsigned int m_TextureNum, m_DepthBufferNum;
	Framebuffer m_Fbo;
	VertexBuffer m_Vbo, m_TexVbo;
	Program* m_Shader;

public:
	void Init();
	void Bind();
	void Unbind();
	void Draw();

	CMirror(Program* shader);
	~CMirror(void);
};

