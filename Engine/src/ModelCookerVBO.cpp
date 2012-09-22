#include "StdAfx.h"
#include "ModelCookerVBO.h"
#include "ModelData.h"
#include "Vector3.h"
#include "Vector2.h"

void ComputeTangentBasis(
      const CVector3& P1, const CVector3& P2, const CVector3& P3, 
      const CVector2<float>& UV1, const CVector2<float>& UV2, const CVector2<float>& UV3,
      CVector3 &tangent, CVector3 &bitangent )
{
   CVector3 Edge1 = P2 - P1;
   CVector3 Edge2 = P3 - P1;
   CVector2<float> Edge1uv = UV2 - UV1;
   CVector2<float> Edge2uv = UV3 - UV1;

   float cp = Edge1uv.Y * Edge2uv.X - Edge1uv.X * Edge2uv.Y;

   if ( cp != 0.0f ) {
      float mul = 1.0f / cp;
      tangent   = (Edge1 * -Edge2uv.Y + Edge2 * Edge1uv.Y) * mul;
      bitangent = (Edge1 * -Edge2uv.X + Edge2 * Edge1uv.X) * mul;

      tangent.Normalize();
      bitangent.Normalize();
   }
}

bool CModelCookerVBO::Cook(const CModelData& modelData)
{
	auto const& Components = modelData.GetComponents();
	auto const& Materials = modelData.GetMaterials();
	auto const& Vertices = modelData.GetVertices();
	auto const& Normals = modelData.GetNormals();
	auto const& TexCoords = modelData.GetTexCoords();

	// Ładujemy dane do odpowiednich VBO
	// Musimy stworzyc wszystkie wierzcholki jako pojedyncze instancje

	// Wierzcholki, normalne i texcoordy sa poukladane wzgledem wystepowania na scianach
	// Kazdy wierzcholek, jaki pojawia sie na scianie, tworzy inna kombinacje V/N/T
	// Kazda z tych kombinacji musi miec swoje pole(indeks)
	// Nie wszystkie sa oczywiscie wykorzystywane, ale i tak rozmiar danych wzrasta do
	// (3+3+2) * ilosc_trojkatow * 3. Poprzednio udawalo sie zaoszczedzic tak naprawde niewiele.

	std::vector<CModelData::SVector3> TempVerts;
	std::vector<CModelData::SVector3> TempNormals;
	std::vector<CModelData::SVector2> TempTexCoords;
	// Do bumpmappingu
	//std::vector<CModelData::SVector3> TempTangents, TempBitangents;

	std::vector<int> TempIndices;
	// Tymczasowe zmienne alokuję na zewnątrz pętli
	CModelData::SVector3 Temp3;
	CModelData::SVector3 Temp2;

	for (auto Cmp = Components.begin(); Cmp != Components.end(); ++Cmp)
	{
		for (auto Grp = Cmp->Groups.begin(); Grp != Cmp->Groups.end(); ++Grp)
		{			
			for (auto Face = Grp->Faces.begin(); Face != Grp->Faces.end(); ++Face)
			{
				// Uzywam 3 buforow, bo sa bardziej elastycznym rozwiazaniem
				// Umozliwiaja np. animacje samych pozycji, pozostawiajac
				// dwa pozostale bufory w spokoju.
				for (int i = 0; i < 3; ++i)
				{
					TempVerts.push_back(Vertices[Face->VertIndices[i]]);
					TempNormals.push_back(Normals[Face->NormIndices[i]]);
					TempTexCoords.push_back(TexCoords[Face->TexCIndices[i]]);
				}
				//CVector3 Tangent, Bitangent;
				//ComputeTangentBasis(reinterpret_cast<CVector3>(
			}
		}
	}

	// Teraz, kiedy juz stworzylismy sobie tymczasowe dane w przestrzeni, mozemy skopiowac
	// je (sic!) do pamieci karty graficznej - przysiegam, ze to juz ostatnie kopiowanie
	m_VerticesVbo.LoadData(&(TempVerts[0]),
		sizeof(CModelData::SVector3) * TempVerts.size());
	m_NormalsVbo.LoadData(&(TempNormals[0]),
		sizeof(CModelData::SVector3) * TempNormals.size());
	m_TexCoordsVbo.LoadData(&(TempTexCoords[0]),
		sizeof(CModelData::SVector2) * TempTexCoords.size());

	// Teraz, kiedy mam juz dane odpowiednio zapakowane do VBO...
	// Tworzymy VAO per komponent
	for (auto Cmp = Components.begin(); Cmp != Components.end(); ++Cmp)
	{
		m_Components.push_back(SCookedComponentVBO(Cmp->Center));
		m_Components.back().VertCount = TempVerts.size();
		// Bindujemy VAO komponentu
		m_Components.back().Vao.Bind();
		// Bindujemy 3 nasze strumyczki danych
		// I teraz uwaga - pierwszy parametr do glVertexAttribPointer
		// teoretycznie powinien zostac pobrany z shadera(programu) 
		// za pomoca glGetAttribLocation(), ale nie mam pojecia, jak to sie odnosi
		// do tworzenia naszego obiektu -.-

		// Kiedys zamiast glVertexAttribPointer byloby np. glVertexPointer.
		// Zostalo to jednak zastapiane przez bardziej ogólną wersję,
		// pozwalająca przekazywać tak naprawdę dowolne dane. Shader
		// wymaga jednak odpowiedniego "nazwania" każdego strumienia danych.
		
		// W tej chwili owo "nazwanie" wykonywane jest przy ladowaniu CShadera

		m_VerticesVbo.Bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		m_NormalsVbo.Bind();
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		m_TexCoordsVbo.Bind();
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}

	return true;
}

void CModelCookerVBO::Draw ()
{
	// Zakładamy, że jedyne co może się zmieniać w grupie, to dane vec4/vec3/float
	// Samplery pozostają stałe co do komponentu
	for (auto Cmp = m_Components.begin(); Cmp != m_Components.end(); ++Cmp)
	{
		Cmp->Vao.Bind();
		// Tutaj wstawic ewentualne glVertexAttrib*()
		// Jak na przyklad transformacje grup w animacji
		glDrawArrays(GL_TRIANGLES, 0, Cmp->VertCount);
	}
}

CModelCookerVBO::CModelCookerVBO(void):
	m_VerticesVbo(CVertexBuffer::DATA_BUFFER, CVertexBuffer::STATIC_DRAW),
	m_NormalsVbo(CVertexBuffer::DATA_BUFFER, CVertexBuffer::STATIC_DRAW),
	m_TexCoordsVbo(CVertexBuffer::DATA_BUFFER, CVertexBuffer::STATIC_DRAW)
{
}


CModelCookerVBO::~CModelCookerVBO(void)
{
}
