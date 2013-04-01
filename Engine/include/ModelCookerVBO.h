#pragma once
#include "IModelCooker.h"
#include "VertexAttributeArray.h"
#include "VertexBuffer.h"
#include <vector>

namespace engine {

class ModelData;

class ModelCookerVBO :
    public ModelCooker
{
private:
    struct CookedComponentVBO :
        public ModelCooker::CookedComponent
    {
        int VertCount;
        std::vector<int> GroupEndIndices;
        VertexAttributeArray Vao;

        explicit CookedComponentVBO(glm::vec3 center) :
            ModelCooker::CookedComponent(std::move(center))
            { }

        CookedComponentVBO (CookedComponentVBO&& other) : 
            ModelCooker::CookedComponent(std::move(other.Center)),
            Vao(std::move(other.Vao))
        { }
    };

    VertexBuffer m_VerticesVbo, m_NormalsVbo, m_TexCoordsVbo;
    // For bumpmapping
    //VertexBuffer m_TangentsVbo, m_BitangentsVbo;
    std::vector<CookedComponentVBO> m_Components;

public:
    bool Cook (const ModelData& data);
    void Draw ();

    ModelCookerVBO(void);
    ~ModelCookerVBO(void);
};

} // namespace engine