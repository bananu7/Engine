#include "Graph.h"
#include <Shader.h>
#include <ResManager.h>
#include <Color.h>
#include <Camera.h>
#include <Vector2.h>
#include <vector>

void CGraph::Draw()
{
	//CShader::DisableAll();
	m_Bounds.X = 0.f;
	m_Bounds.Y = 10.f;

	//glEnable(GL_TEXTURE_2D);
	m_Vao.Bind();
	m_Shader->Bind();
	//m_Shader->SetUniformColor("Color",m_Color);

	// Przesuwamy sie na dol, bo rysujemy do gory (wyzej - wieksze y)
	
	unsigned int DataSize = m_Data.size();
	if (DataSize == 0)
		return;

	m_StepX = 0;

/*	int DrawingCount = (m_StepX != 0) ? (m_Width / m_StepX) : (m_Width / DataSize);
	if (DrawingCount >= DataSize)
		DrawingCount = DataSize-1;*/
	int DrawingCount = (m_Width < DataSize) ? m_Width : DataSize;

	double Density = m_Bounds.Y - m_Bounds.X;

	std::vector<CVector2<float> > Vertices;
	std::vector<CVector3> Colors;

	for (unsigned int i = 0; i < DrawingCount; ++i)
	{
		// Rysujemy ostatie probki z danych
		// Minus, bo rysujemy do góry
		double Y = m_Height - (m_Data[i+DataSize-DrawingCount]) * m_Scale;

		if		(Y<=0)
		{
			Y = 0;
			Colors.push_back(CVector3(.8f,0.f,0.f));
		}
		else if (Y>=m_Height)
		{
			Y = m_Height;
			Colors.push_back(CVector3(1.f,0.f,0.f));
		}
		else
			Colors.push_back(CVector3(m_Color.R, m_Color.G, m_Color.B));
			Vertices.push_back(CVector2<float>(m_X+i,m_Y+Y));
	}

	//glBindBuffer(GL_ARRAY_BUFFER,0);
	//glGenBuffers(1, &m_Vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof Vertices, Vertices, GL_STATIC_DRAW);

	m_NormalsVbo.LoadData(&(Colors[0]), sizeof(float) * 3 * Colors.size());
	m_VerticesVbo.LoadData(&(Vertices[0]), sizeof(float) * 2 * Vertices.size());

	m_NormalsVbo.Bind();
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	m_VerticesVbo.Bind();
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_LINE_STRIP, 0, Vertices.size());
}

void CGraph::AddData (const double data)
{
	if (m_WaitForChange)
	{
		if		(m_Data.empty())
			m_Data.push_back(data);
		else if (m_Data.back() != data)
			m_Data.push_back(data);
	}
	else
	m_Data.push_back(data);
}
void CGraph::AddData (const std::vector<double>& data)
{
	m_Data.insert (m_Data.end(), data.begin(), data.end());
}
void CGraph::AddData (const double* data, unsigned int size)
{
	m_Data.insert (m_Data.end(), data, data+size*sizeof(double));
}

void CGraph::SetScale(float Scale)
{
	m_Scale = Scale;
}
void CGraph::SetIfWaiting(bool IsWaiting)
{
	m_WaitForChange = IsWaiting;
}
void CGraph::SetColor(CColor Color)
{
	m_Color = Color;
}

CGraph::CGraph(float X, float Y, float Width, float Height,
				   CColor Color, float Scale,
				   bool IsWaiting):
	m_Width(Width),
	m_Height(Height),
	m_Color(Color),
	m_Scale(Scale),
	m_WaitForChange(IsWaiting),
	CGUIElement()
{
	m_X= X;
	m_Y= Y;
	m_Shader = CResManager::GetSingleton()->GetResource<CShader>("shader_interface");
	m_Shader->SetUniformMatrix4("ProjectionMatrix",*CGUI::GetSingleton()->GetProjectionMatrix());
	//m_ControlWithFocus = 0;
	//m_LastCommand = m_LastCommands.end();
	//m_LogFile.open ("guilog.txt", ios::out | ios::trunc);
	//m_TextProvider = new CGenericWglText();
}

/*CGraph::~CGraph(void)
{
	//m_LogFile.close();
	//m_TextProvider->Kill();
}*/
