
#include "LightHouse.h"

void CLightHouse::AddLight (SLight light)
{
	m_Lights.push_back(std::move(light));
}

CLightHouse::CLightHouse(void)
{
}


CLightHouse::~CLightHouse(void)
{
}
