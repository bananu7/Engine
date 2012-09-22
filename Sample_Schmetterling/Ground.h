#pragma once
#include "VertexBuffer.h"
#include "VertexAttributeArray.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Resource.h"

class CModel;
class CImageData;

class CGround : 
	public CResource
{
protected:
	class CHeightmap 
	{
	protected:
		CImageData*				m_Data;
		float*					m_Heights;
		CVector2<unsigned int>	m_Size;
		CVector3*				m_Normals;

		void _SmoothTerrain(int passes);

	public:
		inline const CVector2<unsigned int>& GetSize() const { return m_Size; }
		CVector3& GetNormal (unsigned int i, unsigned int k);
		const CVector3& GetNormal (unsigned int i, unsigned int k) const;
		const float* GetRawData () const { return m_Heights; }

		bool Load ();
		float GetHeight (unsigned int i, unsigned int k) const;
	} m_HeightMap;

	CVertexBuffer m_VerticesVbo, m_NormalsVbo, m_TexCoordsVbo;
	CVertexBuffer m_IndicesVbo;
	CVertexAttributeArray m_Vao;

	float	 m_TileSize;
	float m_HScale, m_WScale;

public:
	std::string Load(SLoadParams const& loadParams);
	void Unload() { }
	void Draw();

	float GetHeight (const CVector2<float>& pos) const;
	CVector3 GetNormal (const CVector2<float>& pos) const;
	CVector2<unsigned int> const& GetSize() const { return m_HeightMap.GetSize(); }
	float GetHScale() const { return m_HScale; }
	float GetWScale() const { return m_WScale; }
	const float* GetRawData() const { return m_HeightMap.GetRawData(); }

	CGround(void);
	~CGround(void);
};

