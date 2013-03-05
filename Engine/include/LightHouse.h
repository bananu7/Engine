#pragma once
#include <glm/glm.hpp>
#include <list>

namespace engine {

struct SLight
{
	glm::vec4 Ambient, Diffuse, Specular;
	glm::vec3 Position;
	// float Attennuation;
	// float spotlight cutoff
	// CVector3 spot direction
};

class CLightHouse
{
	std::list<SLight> m_Lights;

public:
	void AddLight (SLight light);

	CLightHouse(void);
	~CLightHouse(void);
};

} // namespace engine