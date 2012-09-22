#pragma once
#include "Entity.h"
class CBullet :
	public CEntity
{
public:
	CBullet(const CEntity* start);
	~CBullet(void);
};

