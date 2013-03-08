#include "Ground.h"
#include "Vector3.h"
#include "ImageData.h"
#include "ResManager.h"
#include "Misc.h"
#include "Shader.h"

#include <GL/glew.h>
#include <fstream>
#include <vector>

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

void CGround::Draw()
{
	auto const& HSize = m_HeightMap.GetSize();
	//m_Vao.Draw(0, (HSize.X-1)*(HSize.Y-1)*6);
	m_Vao.Bind();

	glPointSize(10.0f);
	for (int i = 0; i < HSize.Y-1; ++i)
	{
		glDrawElements(GL_TRIANGLE_STRIP, HSize.X * 2, GL_UNSIGNED_INT, BUFFER_OFFSET((i * HSize.X * 2) * sizeof(unsigned int)));

		/*for (int k = 0; k < 16; ++k)
		{
			glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, BUFFER_OFFSET((i * HSize.X * 2 + k) * sizeof(unsigned int)));
		}*/
	}
	//glDrawRangeElements(GL_POINTS, 0, , HSize.X*2, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_POINTS, 0, 4);
}

std::string CGround::Load(ILoader const& loadParams)
{
	string TempParam;

	m_WScale = 1.f;
	if (loadParams.GetParam("WScale", TempParam))
		m_WScale = boost::lexical_cast<float>(TempParam);

	m_HScale = 100.f;
	if (loadParams.GetParam("HScale", TempParam))
		m_HScale = boost::lexical_cast<float>(TempParam);

	// Zachowujemy "proporcje" mapy
	m_HScale *= m_WScale;

	m_HeightMap.Load();
	auto HSize = m_HeightMap.GetSize();

	float TexScale = 0.1f;

	// Wierzcholkow jest tyle, ile informacji o wysokosci
	int VertCount = HSize.X * HSize.Y;

	float* Vertices = new float[VertCount*3];
	float* Normals = new float[VertCount*3];
	float* TexCoords = new float[VertCount*2];

	memset(Normals, 0, sizeof(float)*VertCount*3);
	memset(TexCoords, 0, sizeof(float)*VertCount*2);

	std::vector<unsigned int> Indices;

	std::ofstream Dump ("ground_dump.txt");

	for (unsigned int i = 0; i < HSize.X; ++i)
	{
		for (unsigned int k = 0; k < HSize.Y; ++k)
		{
			float XPos = i * m_TileSize;
			float YPos = k * m_TileSize;

			CVector3 A(XPos, m_HeightMap.GetHeight(i, k), YPos);
			CVector3 B(XPos + m_TileSize, m_HeightMap.GetHeight(i+1, k), YPos);
			CVector3 C(XPos + m_TileSize, m_HeightMap.GetHeight(i+1, k+1), YPos+m_TileSize);

			CVector3 D (XPos, m_HeightMap.GetHeight(i, k+1), YPos+m_TileSize);

			A.X *= m_WScale;
			A.Y *= m_HScale;
			A.Z *= m_WScale;
			B.X *= m_WScale;
			B.Y *= m_HScale;
			B.Z *= m_WScale;
			C.X *= m_WScale;
			C.Y *= m_HScale;
			C.Z *= m_WScale;

			CVector3 Temp1 = A-B;
			CVector3 Temp2 = B-C;
			Temp1.CrossProduct(Temp2);
			Temp1.Normalize();

			CVector3 Temp3 = A-C;
			CVector3 Temp4 = C-D;
			Temp3.CrossProduct(Temp4);
			Temp3.Normalize();

			m_HeightMap.GetNormal((int)(XPos),(int)(YPos)) = Temp1;

			{
				int Index;
				Index = (i * HSize.Y + k) * 3;
				Vertices[Index+0] = A.X;
				Vertices[Index+1] = A.Y;
				Vertices[Index+2] = A.Z;
				Normals[Index+0]  = -Temp1.X;
				Normals[Index+1]  = -Temp1.Y;
				Normals[Index+2]  = -Temp1.Z;
				Dump << -Temp3.X << " " << -Temp3.Y << " " << -Temp3.Z << std::endl;
			}
			{
				int Index;
				Index = (i * HSize.Y + k) * 2;
				TexCoords[Index+0] = TexScale * i;
				TexCoords[Index+1] = TexScale * k;
			}
		}
	}

	// Tworzymy indeksy wierzchoklow
	for (int y = 0; y < HSize.Y-1; ++y)
	{
		for(int x = 0; x < HSize.X; ++x)		
		{
			// Wersja na 1 drawcall
			// Jesli jestesmy na koncu, odwracamy kolejnosc
			// chyba jakas zmienna by sie przydala do tego,
			// albo w ogole jedna petle

			Indices.push_back(y * HSize.X + x);
			Indices.push_back((y+1) * HSize.X + x);
		}
	}

	Dump << "indices:\n";
	for (auto it = Indices.begin(); it != Indices.end(); ++it)
	{
		Dump << *it << std::endl;
	}

	m_Vao.Bind();

	m_VerticesVbo.Bind();
	glBufferData(GL_ARRAY_BUFFER, VertCount*3*sizeof(float), Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	m_NormalsVbo.Bind();
	glBufferData(GL_ARRAY_BUFFER, VertCount*3*sizeof(float), Normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	m_TexCoordsVbo.Bind();
	glBufferData(GL_ARRAY_BUFFER, VertCount*2*sizeof(float), TexCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	
	m_IndicesVbo.Bind();
	//m_IndicesVbo.LoadData(&(Indices[0]), sizeof(unsigned int)*Indices.size());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size()*sizeof(unsigned int), &(Indices[0]), GL_STATIC_DRAW);

	//m_VerticesVbo.Bind();

	int Err = glGetError();

	delete Vertices;
	delete Normals;
	delete TexCoords;

	return string();
}

CGround::CGround(void):
	m_VerticesVbo(VertexBuffer::DATA_BUFFER, VertexBuffer::STATIC_DRAW),
	m_NormalsVbo(VertexBuffer::DATA_BUFFER, VertexBuffer::STATIC_DRAW),
	m_TexCoordsVbo(VertexBuffer::DATA_BUFFER, VertexBuffer::STATIC_DRAW),
	m_IndicesVbo(VertexBuffer::INDEX_BUFFER, VertexBuffer::STATIC_DRAW)
{
	m_TileSize = 1.f;
}


CGround::~CGround(void)
{
}

bool CGround::CHeightmap::Load ()
{
	m_Data = CResManager::GetSingleton()->GetResource<CImageData>("heightmap");
	m_Size.X = m_Data->GetWidth();
	m_Size.Y = m_Data->GetWidth();
	m_Heights = new float [m_Size.X*m_Size.Y];
	/*for (int i = 0; i < m_Size.X; ++i)
	{
		for (int k = 0; k < m_Size.Y; ++k)
		{
			float PixPart = (float)(m_Data->GetPixel(i, k).r);
			m_Heights[k+i*m_Size.Y] = (PixPart - 127.f);
		}
	}*/
	for (int i = 0; i < m_Size.Y; ++i)
	{
		for (int k = 0; k < m_Size.X; ++k)
		{
			float PixPart = (float)(m_Data->GetPixel(k, i).R) / 255.f;
			m_Heights[k+i*m_Size.X] = (PixPart);
		}
	}
	_SmoothTerrain (25);
	m_Normals = new CVector3 [m_Size.X*m_Size.Y];
	return true;
}

void CGround::CHeightmap::_SmoothTerrain(int passes)
{
   float* NewHeightData;

   while (passes--)
   {
	   // Note: m_Size.X and m_Size.Y should be equal and power-of-two values 
	   NewHeightData = new float[m_Size.X * m_Size.Y];

	   for (int x = 0; x < m_Size.X; x++)
	   {
		  for (int y = 0; y < m_Size.Y; y++)
		  {
			  int adjacentSections = 0;
			  float sectionsTotal = 0.0f;

			  if ((x - 1) > 0) // Check to left
			  {
				 sectionsTotal += m_Heights[(x - 1)*m_Size.Y + y];
				 adjacentSections++;

				 if ((y - 1) > 0) // Check up and to the left
				 {
					sectionsTotal += m_Heights[(x - 1)*m_Size.Y + y - 1];
					adjacentSections++;
				 }

				 if ((y + 1) < m_Size.Y) // Check down and to the left
				 {
					sectionsTotal += m_Heights[(x - 1)*m_Size.Y + y + 1];
					adjacentSections++;
				 }
			  }

			  if ((x + 1) < m_Size.X) // Check to right
			  {
				 sectionsTotal += m_Heights[(x + 1)*m_Size.Y + y];
				 adjacentSections++;

				 if ((y - 1) > 0) // Check up and to the right
				 {
					 sectionsTotal += m_Heights[(x + 1) * m_Size.Y + y - 1];
					 adjacentSections++;
				 }

				 if ((y + 1) < m_Size.Y) // Check down and to the right
				 {
					 sectionsTotal += m_Heights[(x + 1)*m_Size.Y + y + 1];
					 adjacentSections++;
				 }
			  }

			  if ((y - 1) > 0) // Check above
			  {
				 sectionsTotal += m_Heights[x*m_Size.Y + y - 1];
				 adjacentSections++;
			  }

			  if ((y + 1) < m_Size.Y) // Check below
			  {
				 sectionsTotal += m_Heights[x*m_Size.Y + y + 1];
				 adjacentSections++;
			  }

			  NewHeightData[x*m_Size.Y + y] = (m_Heights[x*m_Size.Y + y] + (sectionsTotal / adjacentSections)) * 0.5f;
		   }
	   }
	   delete m_Heights;
	   m_Heights = NewHeightData;
   }
}

float CGround::CHeightmap::GetHeight (unsigned int i, unsigned int k) const
{
	if (i < m_Size.X && k < m_Size.Y)
		return m_Heights[i*m_Size.Y + k];
	else
		return 0.f;
}

CVector3& CGround::CHeightmap::GetNormal (unsigned int i, unsigned int k)
{
	return m_Normals[i * m_Size.Y + k];
}

const CVector3& CGround::CHeightmap::GetNormal (unsigned int i, unsigned int k) const
{
	return m_Normals[i * m_Size.Y + k];
}

float CGround::GetHeight (const CVector2<float>& pos) const
{
	CVector2<float> Pos = pos / m_WScale;

	float InterpX = DotPart(Pos.X/m_TileSize);
	float InterpY = DotPart(Pos.Y/m_TileSize);
	bool TopTriangle = InterpY > InterpX;
	int PosX = (int)(Pos.X/m_TileSize), PosY = (int)(Pos.Y/m_TileSize);
	float Ah = m_HeightMap.GetHeight(PosX, PosY), Bh, Ch, Dh;
	float TempH1, TempH2;

	float Ret;
	if (TopTriangle)
	{
		Bh = m_HeightMap.GetHeight(PosX, PosY+1);	// 0,1
		Ch = m_HeightMap.GetHeight(PosX+1, PosY+1); // 1,1
		Dh = Ch + Ah - Bh;				// 1,0

		TempH1 = Dh*InterpX + Ah*(1.f-InterpX);
		TempH2 = Ch*InterpX + Bh*(1.f-InterpX);
		Ret = TempH2*InterpY + TempH1*(1.f-InterpY);
	}
	else
	{
		Bh = m_HeightMap.GetHeight(PosX+1, PosY);	// 1,0
		Ch = m_HeightMap.GetHeight(PosX+1, PosY+1);	// 1,1
		Dh = Ah + Ch - Bh;							// 0,1

		TempH1 = Bh*InterpX + Ah*(1.f-InterpX);
		TempH2 = Ch*InterpX + Dh*(1.f-InterpX);
		Ret = TempH2*InterpY + TempH1*(1.f-InterpY);
	}

	/*CGUI::GetSingleton()->SetDebugField("Ah", Ah);
	CGUI::GetSingleton()->SetDebugField("Bh", Bh);
	CGUI::GetSingleton()->SetDebugField("Ch", Ch);
	CGUI::GetSingleton()->SetDebugField("Dh", Dh);
	CGUI::GetSingleton()->SetDebugField("TempH1", TempH1);
	CGUI::GetSingleton()->SetDebugField("TempH2", TempH2);
	CGUI::GetSingleton()->SetDebugField("InterpX", InterpX);
	CGUI::GetSingleton()->SetDebugField("InterpY", InterpY);*/

	return Ret * m_HScale;
}

CVector3 CGround::GetNormal (const CVector2<float>& pos) const
{
	return m_HeightMap.GetNormal((int)(pos.X),(int)(pos.Y));
}
