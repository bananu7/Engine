#include "Mirror.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "VertexAttributeArray.h"
#include "Matrix4.h"

void CMirror::Init()
{
    glGenTextures(1, &m_TextureNum);
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, m_TextureNum);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, 1024, 720, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Depth renderbuffer
    glGenRenderbuffers(1, &m_DepthBufferNum);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferNum);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 720);

    m_Fbo.AttachRenderBuffer(m_DepthBufferNum, GL_DEPTH_ATTACHMENT);
    m_Fbo.AttachTexture(GL_TEXTURE_2D, m_TextureNum);

    static float Vertices[] =
    {
        0.f, 150.f, 0.f,
        50.f, 150.f, 0.f,
        50.f, 200.f, 0.f,
        0.f, 200.f, 0.f
    };

    static float TexCoords[] = 
    {
        0.f, 0.f,
        1.f, 0.f, 
        1.f, 1.f, 
        0.f, 1.f
    };
    
    m_Vbo.Bind();
    m_Vbo.LoadData(Vertices, sizeof(float) * 3 * 4);
    m_TexVbo.Bind();
    m_TexVbo.LoadData(TexCoords, sizeof(float) * 2 * 4);

    m_Shader->SetTex("Screen", 0);
    m_Shader->SetUniform("ModelMatrix", CMatrix4::CreateIdentity());
}

void CMirror::Bind()
{
    m_Fbo.Bind();
    GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    //glClearColor(0.f, 0.f, 0.f, 1.f);
    //glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CMirror::Unbind()
{
    Framebuffer::Disable();
}

void CMirror::Draw()
{
    glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_2D, m_TextureNum);
    
    glBindVertexArray(0);
    
    Framebuffer::Disable();
    m_Shader->Bind();

    m_Vbo.Bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    m_TexVbo.Bind();
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    //glDrawElements(GL_QUADS, 4, GL_FLOAT, 0);
    glDrawArrays(GL_QUADS, 0, 4);
}

CMirror::CMirror(Program* shader):
    m_Vbo(VertexBuffer::DATA_BUFFER, VertexBuffer::STATIC_DRAW),
    m_TexVbo(VertexBuffer::DATA_BUFFER, VertexBuffer::STATIC_DRAW),
    m_Shader(shader)
{
}


CMirror::~CMirror(void)
{
}
