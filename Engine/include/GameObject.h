#pragma once
#include <glm/glm.hpp>
// TODO :model fizyczny |  #include "Entity.h"

namespace engine {

class Sprite;
class AnimationSet;

class GameObject
{
private:
    unsigned m_Id; // unikalny per poziom

protected:
    // TODO : model fizyczny | CEntity* m_Body;
    AnimationSet* m_AnimSet;
    Sprite* m_Sprite;
    float m_Direction;
    float m_AnimationProgress;
    unsigned m_AnimationState;
    unsigned m_Kind;
    std::string m_Type;

public:
    virtual void Update (const float dt);
    virtual void Draw ();

    //virtual glm::vec3 GetPosition () const { return m_Body->GetPosition(); }
    //virtual glm::vec3 GetVelocity () const { return m_Body->GetVelocity(); }
    virtual float GetRotation () const { return m_Direction; }
    virtual unsigned GetId () const { return m_Id; }

    virtual inline void SetPosition (const glm::vec3& pos){ } //{ m_Body->SetPosition(pos); }
    virtual void        SetSize (const glm::vec3& size);
    virtual void        SetAddPosition (const glm::vec3 &pos);
    virtual inline void SetVelocity (const glm::vec3& vel){ }// { m_Body->SetVelocity(vel); }
    virtual inline void SetDirection (const float direction) { m_Direction = direction; }
    inline void SetAnimationState (unsigned animState) { m_AnimationState = animState; }

    GameObject(unsigned id, const std::string& type, unsigned kind);
    virtual ~GameObject(void);
};

} // namespace engine