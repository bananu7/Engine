
#include "GameObject.h"
#include "Image.h"
#include "ResManager.h"
//#include "AnimationSet.h"
//#include "Physics.h"
#include <boost/lexical_cast.hpp>

using namespace std;

void CGameObject::Update (const float dt)
{
	m_AnimationProgress += dt;
}

void CGameObject::Draw()
{
	/*glEnable (GL_TEXTURE_2D);
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	float X = m_Body->GetPosition().X*CGraphics::GetSingleton()->GetDisplayScaleFactor();
	float Y = m_Body->GetPosition().Y*CGraphics::GetSingleton()->GetDisplayScaleFactor();*/

	// TODO animacje
	//const CImage* TempImg = m_AnimSet->GetActiveFrame (m_AnimationState, m_AnimationProgress);

	/* TODO zamienic na model
	m_Sprite->SetImage (TempImg);
	m_Sprite->SetPosition (GetPosition());
	m_Sprite->SetRotation (GetRotation());

	m_Sprite->Draw();*/
}

void CGameObject::SetAddPosition (const CVector3& pos)
{
	//CVector2<float> BodyPos = m_Body->GetPosition();
	//BodyPos += pos;
	//m_Body->SetPosition (BodyPos);
}

void CGameObject::SetSize (const CVector3& size)
{
	//m_Body->SetSize(size);
	//m_Sprite->SetSize(size);
}

CGameObject::CGameObject(uint32 id, const string& type, uint32 kind) :
	m_Id(id), m_Type(type), m_Kind(kind)
{
	//ESS : Type + kind + animset
	/*m_AnimSet = CResManager::GetSingleton()->GetResource<CAnimationSet>(type + boost::lexical_cast<string,uint32>(kind) + "_anim_set");
	m_AnimationProgress = 0.0f;
	m_AnimationState = 0;
	m_Body = CPhysics::GetSingleton()->CreateEntity();
	//CPhysics::GetSingleton()->AddEntity(m_Body);
	CVector2<float> TempPosition (8.0f, 5.0f);
	CVector2<float> TempSize (1.f, 1.f);
	m_Direction = 0.0f;
	m_Body->SetSize (TempSize);
	m_Body->SetPosition (TempPosition);
	m_Sprite = new CSprite();
	m_Sprite->SetSize (TempSize);
	m_Sprite->SetDrawAtCenter(false);*/
}

CGameObject::~CGameObject(void)
{
}
