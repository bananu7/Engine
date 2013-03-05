#pragma once
#include <singleton.h>
#include <Shader.h>
#include <VertexBuffer.h>
#include <VertexAttributeArray.h>
#include <Matrix4.h>
#include <Color.h>
#include <map>
#include <utility>

class CGUIElement
{
protected:
	bool					m_Hidden;
	int						m_ID;
	float					m_X,m_Y;
	Program*				m_Shader;
	CVertexBuffer			m_VerticesVbo;
	CVertexBuffer			m_NormalsVbo;
	CVertexBuffer			m_UVsVbo;
	CVertexAttributeArray	m_Vao;

public:
	virtual void Draw() { } // must be declared in child!

	int GetID();
	float GetPositionX();
	float GetPositionY();
	void Move(float X, float Y);
	bool IsHidden() { return m_Hidden; }
	void Hide() { m_Hidden = 1; }
	void Show() { m_Hidden = 0; }

	friend class CGUI;
	friend class CBox;
	friend class CFont;
	friend class CGraph;

	CGUIElement():
		m_VerticesVbo(CVertexBuffer::DATA_BUFFER, CVertexBuffer::STATIC_DRAW),
		m_NormalsVbo(CVertexBuffer::DATA_BUFFER, CVertexBuffer::STATIC_DRAW),
		m_UVsVbo(CVertexBuffer::DATA_BUFFER, CVertexBuffer::STATIC_DRAW)
	{
		Show();
	}
	//~CGUIElement(void);
};

//forward declaration of CGUIElement children
class CBox;
class CFont;
class CGraph;

class CGUI :
	public CSingleton<CGUI>
{
protected:
	int							m_ElementsCount;
	float						m_Width;
	float						m_Height;
	std::map<int,CGUIElement*>	m_Elements;
	CMatrix4					m_Ortho;

public:
	void Draw();

	template <typename T> T* AddElement(const T &Data)
	{
		T* TempPtr = new T(Data);
		CGUIElement* TempBasePtr = dynamic_cast<CGUIElement*>(TempPtr);
		//T* TempBasePtr = TempPtr;
		if (TempBasePtr)
		{
			TempBasePtr->m_ID = m_ElementsCount++;
			TempBasePtr->m_Hidden= 0;
			m_Elements.insert
				(std::make_pair(TempBasePtr->GetID(),TempBasePtr));
			return TempPtr;
		}
		return false;
	}
	template <typename T> T* AddElement()
	{
		T* TempPtr = new T();
		TempPtr->m_ID = m_ElementsCount++;
		m_Elements.insert
			(std::make_pair(m_ElementsCount,TempPtr));
		return TempPtr;
	}
	//template <typename T> bool AddElement();
	bool RemoveElement(int ID);
	CGUIElement* GetElement(int ID);

	float GetWidth();
	float GetHeight();
	CMatrix4* GetProjectionMatrix();

	bool Inititalise(float Width, float Height);
	CGUI();
	//~CGUI(void);

};

class CBox:
	public CGUIElement
{
protected:
	CColor						m_Color;
	float						m_Width;
	float						m_Height;

public:
	void Draw();
	float GetWidth();
	float GetHeight();
	CColor GetColor();

	void SetColor(CColor Color);
	void Move(float X, float Y);
	void Resize(float Width, float Height);

	CBox(float X, float Y,
		 float Width, float Height,
		 CColor Color);
};