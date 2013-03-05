#pragma once
#include <glm/glm.hpp>
#include <memory>

namespace engine {

class CModelData;

// Interfejs do cookowania danych modelu - czyli zamiany ich na postac, ktora
// bedzie latwo wyswietlana z OpenGL. Implementowany np. przez CModelCookerDisplayList.
class IModelCooker //interface
{
protected:
	struct SCookedComponent 
	{
		//CTransform	Transform;
		float Scale;
		const glm::vec3 Center;

		SCookedComponent(glm::vec3 center) :
			Center(std::move(center)) { }
	};

public:
	virtual bool Cook (const CModelData& data) = 0;
	virtual void Draw () = 0;

	IModelCooker(void) { }
	virtual ~IModelCooker(void) { }
};

} // namespace engine