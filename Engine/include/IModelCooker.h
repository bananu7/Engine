#pragma once
#include <glm/glm.hpp>
#include <memory>

namespace engine {

class ModelData;

// Interfejs do cookowania danych modelu - czyli zamiany ich na postac, ktora
// bedzie latwo wyswietlana z OpenGL. Implementowany np. przez CModelCookerDisplayList.
class ModelCooker //interface
{
protected:
	struct CookedComponent 
	{
		//CTransform	Transform;
		float Scale;
		const glm::vec3 Center;

		CookedComponent(glm::vec3 center) :
			Center(std::move(center)) { }
	};

public:
	virtual bool Cook (const ModelData& data) = 0;
	virtual void Draw () = 0;

	ModelCooker(void) { }
	virtual ~ModelCooker(void) { }
};

} // namespace engine