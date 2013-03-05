#pragma once
#include "IModelCooker.h"
#include <vector>
#include <string>

namespace engine {

class CModelData;

// Implementacja cookera modeli pod OpenGL < 2.0, bez shaderow.
class CModelCookerDisplayList 
	: public IModelCooker
{
private:
	struct SCookedComponentDisplayList :
		public IModelCooker::SCookedComponent
	{
		unsigned int DisplayListNum;

		SCookedComponentDisplayList(glm::vec3 center, unsigned int dslistnum) :
			IModelCooker::SCookedComponent(std::move(center)),
			DisplayListNum(dslistnum) { }
	};

	std::vector<SCookedComponentDisplayList> m_Components;

public:
	bool Cook (const CModelData& data);
	void Draw ();

	CModelCookerDisplayList(void);
	~CModelCookerDisplayList(void);
};

} // namespace engine