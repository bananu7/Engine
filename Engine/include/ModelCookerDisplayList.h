#pragma once
#include "IModelCooker.h"
#include <vector>
#include <string>

namespace engine {

class ModelData;

// Implementacja cookera modeli pod OpenGL < 2.0, bez shaderow.
class ModelCookerDisplayList 
	: public ModelCooker
{
private:
	struct CookedComponentDisplayList :
		public ModelCooker::CookedComponent
	{
		unsigned int DisplayListNum;

		CookedComponentDisplayList(glm::vec3 center, unsigned int dslistnum) :
			ModelCooker::CookedComponent(std::move(center)),
			DisplayListNum(dslistnum) { }
	};

	std::vector<CookedComponentDisplayList> m_Components;

public:
	bool Cook (const ModelData& data);
	void Draw ();

	ModelCookerDisplayList(void);
	~ModelCookerDisplayList(void);
};

} // namespace engine