#pragma once
#include "Typedefs.h"
#include "Vector3.h"
// TODO :model fizyczny |  #include "Entity.h"

class CSprite;
class CAnimationSet;

class CGameObject
{
private:
	unsigned		m_Id; // unikalny per poziom

protected:
	// TODO : model fizyczny | CEntity*		m_Body;
	CAnimationSet*	m_AnimSet;
	CSprite*		m_Sprite;
	float			m_Direction;
	float			m_AnimationProgress;
	uint16			m_AnimationState;
	unsigned			m_Kind;
	std::string		m_Type;

public:
	virtual void Update (const float dt);
	virtual void Draw ();

	//virtual CVector3 GetPosition () const { return m_Body->GetPosition(); }
	//virtual CVector3 GetVelocity () const { return m_Body->GetVelocity(); }
	virtual float			GetRotation () const { return m_Direction; }
	virtual unsigned			GetId		() const { return m_Id; }

	virtual inline void SetPosition (const CVector3& pos){ } //{ m_Body->SetPosition(pos); }
	virtual void		SetSize (const CVector3& size);
	virtual void		SetAddPosition (const CVector3 &pos);
	virtual inline void SetVelocity (const CVector3& vel){ }// { m_Body->SetVelocity(vel); }
	virtual inline void SetDirection (const float direction) { m_Direction = direction; }
	inline void SetAnimationState (uint16 animState) { m_AnimationState = animState; }

	CGameObject(unsigned id, const std::string& type, unsigned kind);
	virtual ~CGameObject(void);
};

