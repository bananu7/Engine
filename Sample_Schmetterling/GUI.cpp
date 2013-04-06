#include "GUI.h"
#include <Camera.h>
#include <Color.h>
#include <Shader.h>
#include <ResManager.h>
#include <Vector3.h>
#include <vector>

void CGUI::Draw()
{
    //glDisable (GL_TEXTURE_2D);
    //Program::DisableAll();
    for (auto it= m_Elements.begin(), itE= m_Elements.end(); it!=itE; ++it)
        (*it).second->Draw();
    //glEnable (GL_TEXTURE_2D);
}

bool CGUI::RemoveElement(int ID)
{
    auto It = m_Elements.find(ID);
    if (It == m_Elements.end())
        return false;
    m_Elements.erase(It);
    return true;
}

CGUIElement* CGUI::GetElement(int ID)
{
    auto it = m_Elements.find(ID);
    auto itE = m_Elements.end();
    if (it==itE) return false;

    return it->second;
}

float CGUI::GetWidth()
{
    return m_Width;
}
float CGUI::GetHeight()
{
    return m_Height;
}
CMatrix4* CGUI::GetProjectionMatrix()
{
    return &m_Ortho;
}

bool CGUI::Inititalise(float Width, float Height)
{
    m_Width = Width;
    m_Height = Height;
    m_Ortho = (Camera::CreateOrtho(0,0,m_Width,m_Height,-1,1));
    return true;
}
CGUI::CGUI():
    m_Ortho(Camera::CreateOrtho(0,0,m_Width,m_Height,-1,1))
{
    m_ElementsCount = 0;
}

int CGUIElement::GetID()
{
    return m_ID;
}
float CGUIElement::GetPositionX()
{
    return m_X;
}
float CGUIElement::GetPositionY()
{
    return m_Y;
}
void CGUIElement::Move(float X, float Y)
{
    m_X = X;
    m_Y = Y;
}

void CBox::Draw()
{
    m_Vao.Bind();

    m_Shader->Bind();    

    std::vector<CVector2<float>> Vertices;
    std::vector<CVector3> Normals;
    
    Vertices.push_back(CVector2<float>(m_X,m_Y));
    Vertices.push_back(CVector2<float>(m_X + m_Width,m_Y));
    Vertices.push_back(CVector2<float>(m_X,m_Y + m_Height));
    Vertices.push_back(CVector2<float>(m_X + m_Width,m_Y + m_Height));
    
    
    for (int i= 0, iE= Vertices.size(); i<iE; ++i)
        Normals.push_back(CVector3(m_Color.R, m_Color.G, m_Color.B));

    m_NormalsVbo.Bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * Normals.size(), &(Normals[0]), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    m_VerticesVbo.Bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * Vertices.size(), &(Vertices[0]), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, Vertices.size());
    
    // old opengl
    /*glBegin(GL_QUADS);
        glVertex2i (m_X, m_Y);
        glVertex2i (m_X + m_Width, m_Y);
        glVertex2i (m_X + m_Width, m_Y + m_Height);
        glVertex2i (m_X, m_Y + m_Height);
    glEnd();*/
}
float CBox::GetWidth()
{
    return m_Width;
}
float CBox::GetHeight()
{
    return m_Height;
}
CColor CBox::GetColor()
{
    return m_Color;
}

void CBox::SetColor(CColor Color)
{
    m_Color = Color;
}
void CBox::Resize(float Width, float Height)
{
    m_Width = Width;
    m_Height = Height;
}

CBox::CBox(float X, float Y,
          float Width, float Height,
          CColor Color):
          m_Width(Width),
          m_Height(Height),
          m_Color(Color),
          CGUIElement()
{
    m_X= X;
    m_Y= Y;
    m_Shader = CResManager::GetSingleton()->GetResource<Program>("shader_interface");
    m_Shader->SetUniform("ProjectionMatrix",*CGUI::GetSingleton()->GetProjectionMatrix());
}