#include "CameraAdds.h"
#include "Misc.h"

using glm::vec3;
using glm::mat4;

void CCameraSimple::CalculateView()
{
	m_ViewMatrix = CreateModelview(Position, Target);
}

void CCameraFly::CalculateView()
{
	mat4 RMatrix = CCamera::CreateRotation(LookDir.y, LookDir.x, 0.f);
	mat4 TMatrix = CCamera::CreateTranslation(-Position.x, -Position.y, -Position.z);
	m_ViewMatrix = mat4();
	m_ViewMatrix *= RMatrix;
	m_ViewMatrix *= TMatrix;
}
void CCameraFly::Fly (float len)
{
	float Pi = 3.1416;
	//Position.X += cosf(LookDir.X - Pi/2.f) * len;
	//Position.Z += sinf(LookDir.X - Pi/2.f) * len;
	mat4 RMatrix = CCamera::CreateRotation(LookDir.x, LookDir.y, 0.f);

	vec3 Norm (0.f, 0.f, 1.f);
	vec3 Delta;
	// FIXME
	//Delta.y = RMatrix.Data[8];
	//Delta.x = RMatrix.Data[9];
	//Delta.z = RMatrix.Data[10];

	Delta *= -len;
	Position += Delta;
}

void CCameraFly::Strafe(float left)
{
	Position.x -= cosf(LookDir.x) * left;
	Position.z -= sinf(LookDir.x) * left;
}

void CCameraTrack::CalculateView()
{
	vec3 Delta;
	Delta.y = 5.0f;
	Delta.x = Radius * cosf(Direction/* + M_PI * .5f*/);
	Delta.z = Radius * sinf(Direction/* + M_PI * .5f*/);
	m_ViewMatrix = CCamera::CreateModelview(Delta + Target, Target);
}