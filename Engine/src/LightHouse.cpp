
#include "LightHouse.h"

namespace engine {

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

} // namespace engine
