#include "Font.h"
#include <Color.h>
#include <ResManager.h>
#include <Image.h>
#include <Camera.h>
#include <Shader.h>
#include <vector>
#include <Vector2.h>
#include <Matrix4.h>
#include <cmath>

using std::string;

void CFont::Draw()
{
	glEnable(GL_TEXTURE_2D);
	//glDisable
	m_Vao.Bind();
	m_Shader->Bind();
	m_Font->Bind(0);
	m_Shader->SetTex("Texture",0);
	m_Shader->SetUniformColor("Color",m_Color);

	std::vector<CVector2<float>> Vertices (4,CVector2<float>(0.f,0.f));
	std::vector<CVector2<float>> UVs (4,CVector2<float>(0.f,0.f));

	int LineNumber= 0;
	/*
	Vertices.push_back(CVector2<float>(m_X,m_Y));
	Vertices.push_back(CVector2<float>(m_X + m_Scale.X,m_Y));
	Vertices.push_back(CVector2<float>(m_X,m_Y + m_Scale.Y));
	Vertices.push_back(CVector2<float>(m_X + m_Scale.X,m_Y + m_Scale.Y));
	*/
	

	for (int i= 0, iE= m_Content.size(); i<iE; ++i)
	{
		if(m_Content[i]=='\n' || i==0)
		{
			++LineNumber;
			Vertices[0] = CVector2<float>(m_X,				m_Y + m_Scale.Y * (LineNumber - 1));
			Vertices[1] = CVector2<float>(m_X + m_Scale.X,	m_Y + m_Scale.Y * (LineNumber - 1));
			Vertices[2] = CVector2<float>(m_X,				m_Y + m_Scale.Y * (LineNumber));
			Vertices[3] = CVector2<float>(m_X + m_Scale.X,	m_Y + m_Scale.Y * (LineNumber));
		}
		if(m_Content[i]!='\n')
		{
			const float SymbolSize = 16.f;
			const float SymbolMargin = 0.05f / 4;

			float XPos = float(int(m_Content[i] % 16));
			float YPos = float(int(m_Content[i] / 16));

			UVs[0] = CVector2<float>(XPos + SymbolMargin,		YPos + SymbolMargin);
			UVs[1] = CVector2<float>(XPos - SymbolMargin + 1,	YPos + SymbolMargin);
			UVs[2] = CVector2<float>(XPos + SymbolMargin,		YPos - SymbolMargin + 1);
			UVs[3] = CVector2<float>(XPos - SymbolMargin + 1,	YPos - SymbolMargin + 1);
			for(int i=0; i<4; ++i)
			{
				UVs[i].X/= SymbolSize;
				UVs[i].Y/= SymbolSize;
			}
		
			m_UVsVbo.Bind();
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * UVs.size(), &(UVs[0]), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);

			m_VerticesVbo.Bind();
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * Vertices.size(), &(Vertices[0]), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_TRIANGLE_STRIP, 0,  Vertices.size());

			for (int k= 0, kE= Vertices.size(); k<kE; ++k)
				Vertices[k].X+= m_Scale.X;
		}
	}
}
void CFont::SetColor(CColor Color)
{
	m_Color = Color;
}
void CFont::Push(const string &String)
{
	m_Content+= String;
}
void CFont::Set(const string &String)
{
	m_Content= String;
}

CFont::CFont(float X, float Y,const string &FontName, CColor Color, CVector2<float> FontScale):

	m_Scale(FontScale),
	m_Color(Color),
	CGUIElement()
{
	m_X = X;
	m_Y = Y;
	m_Font	= CResManager::GetSingleton()->GetResource<CImage>(FontName);
	m_Shader= CResManager::GetSingleton()->GetResource<CShader>("shader_font");
	m_Shader->SetUniformMatrix4("ProjectionMatrix",*CGUI::GetSingleton()->GetProjectionMatrix());
}