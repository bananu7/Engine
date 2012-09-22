#pragma once
#include "Vector3.h"

class CTransform
{
public:
	CVector3 Translation;
	CVector3 Rotation;

	void operator+= (const CTransform& other);

	void ApplyToOpenGL () const;

	CTransform(void);
	CTransform (const CVector3& translation, const CVector3& rotation) :
		Translation(translation), Rotation(rotation) { }
	~CTransform(void);
};

// Do celow dydaktycznych:
/*

TTrans, TRot, TScale dziedzicza po ITransformElement,
posiadajacym metode ExecuteToOpenGL

template<typename TTrans, typename TRot, typename TScale>
class CTransform
{
protected:
	TTrans	m_Translation;
	TRot	m_Rotation;
	TScale	m_Scale;
	(...)
}; */
