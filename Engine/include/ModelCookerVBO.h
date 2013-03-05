#pragma once
#include "IModelCooker.h"
#include "VertexAttributeArray.h"
#include "VertexBuffer.h"
#include <vector>

namespace engine {

class CModelCookerVBO :
	public IModelCooker
{
private:
	struct SCookedComponentVBO :
		public IModelCooker::SCookedComponent
	{
		int VertCount;
		std::vector<int> GroupEndIndices;
		CVertexAttributeArray Vao;

		explicit SCookedComponentVBO(glm::vec3 center) :
			IModelCooker::SCookedComponent(std::move(center))
			{ }
	};

	CVertexBuffer m_VerticesVbo, m_NormalsVbo, m_TexCoordsVbo;
	// For bumpmapping
	//CVertexBuffer m_TangentsVbo, m_BitangentsVbo;
	std::vector<SCookedComponentVBO> m_Components;

public:
	bool Cook (const CModelData& data);
	void Draw ();

	CModelCookerVBO(void);
	~CModelCookerVBO(void);
};

} // namespace engine