#pragma once
#include "Color.h"
#include "Vector3.h"
#include <list>

struct SLight
{
	CColor Ambient, Diffuse, Specular;
	CVector3 Position;
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

