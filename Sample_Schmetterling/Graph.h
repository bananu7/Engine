#pragma once
#include "GUI.h"
#include <Vector2.h>
#include <vector>
#include <Color.h>

class CGraph :
	public CGUIElement
{
protected:
	float					m_Width;
	float					m_Height;
	float					m_Scale;
	bool					m_WaitForChange;
	CColor					m_Color;

	std::vector<double>		m_Data;		//!< \protected Stored data
	CVector2<double>		m_Bounds;	//!< \protected Bounds of graph
	unsigned				m_StepX;	//!< \protected Step of graph

public:
	void CGraph::Draw();

	void AddData (const double data);
	void AddData (const std::vector<double>& data);
	void AddData (const double* data, unsigned int size);

	void SetScale(float Scale);
	void SetIfWaiting(bool IsWaiting);	// if bool == 1, displays a series of the same value as one dot
	void SetColor(CColor Color);

	CGraph(float X, float Y, float Width, float Height,
		   CColor Color, float Scale = 1.f,
		   bool IsWaiting = false);
	//~CGraph(void);
};