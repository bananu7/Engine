#pragma once
#include "Option.h"
#include <vector>

enum EUpdateDataType
{
	PLAYER_MOVEMENT,
	PLAYER_ATTACK,
	PLAYER_DIE,
	MOB_ATTACK,
	MOB_DIE,
	TEST
};

struct SUpdateData
{
	EUpdateDataType			Type;
	std::vector<SOption>	Params;
	SUpdateData (EUpdateDataType type, const std::vector<SOption>& params):
		Type(type), Params(params) { }
};
