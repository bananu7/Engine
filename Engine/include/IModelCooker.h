#pragma once
#include "Transform.h"

class CModelData;

// Interfejs do cookowania danych modelu - czyli zamiany ich na postac, ktora
// bedzie latwo wyswietlana z OpenGL. Implementowany np. przez CModelCookerDisplayList.
class IModelCooker //interface
{
protected:
	struct SCookedComponent 
	{
		CTransform	Transform;
		float		Scale;
		const CVector3 Center;

		SCookedComponent(CVector3 center) :
			Center(std::move(center)) { }
	};

public:
	virtual bool Cook (const CModelData& data) = 0;
	virtual void Draw () = 0;

	IModelCooker(void) { }
	virtual ~IModelCooker(void) { }
};

