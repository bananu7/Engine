#pragma once
#include <vector>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include "Resource.h"

namespace engine {

class ModelData
{
public:
// TODO - zamienic na definicje cross-compiler
#pragma pack (show)
#pragma pack (push, 1) 
    struct Vector3 { float X, Y, Z; Vector3() { } };
    struct Vector2 { float U, V;  };

    struct Face
    {
        unsigned int VertIndices[3]; // Obslugujemy tylko modele triangulizowane
        unsigned int NormIndices[3];
        unsigned int TexCIndices[3];
    };
#pragma pack (pop)
    struct Material 
    {
        float Specular[4], Ambient[4], Diffuse[4]; 
        std::vector<int> Textures;
        Material() { }
    };

    struct SGroup
    {
        std::string MaterialName;
        std::vector<Face> Faces;

        SGroup (std::string matName) :
            MaterialName(std::move(matName)) { }
    };
    
    struct SComponent
    {
        unsigned int Number;
        glm::vec3 Center;
        std::string Name;
        std::vector<SGroup> Groups;

        SComponent (std::string name) :
            Name(std::move(name)),
            Center(glm::vec3(0.f)) { }
    };

protected:
    std::vector<Vector3> m_Vertices;
    std::vector<Vector3> m_Normals;
    std::vector<Vector2> m_TexCoords;
    std::map<std::string, Material> m_Materials;
    std::vector<SComponent> m_Components;

    bool _LoadFromFBX(const std::string &path);
    bool _LoadFromObj (std::istream& str);
    bool _LoadMtl (std::istream& str);

public:
    //FIXME
    template<typename Range>
    static ModelData Load(Range r) { return ModelData(); }

    inline const std::vector<Vector3>& GetVertices() const { return m_Vertices; }
    inline const std::vector<Vector3>& GetNormals() const { return m_Normals; }
    inline const std::vector<Vector2>& GetTexCoords() const { return m_TexCoords; }
    inline const std::map<std::string, Material>& GetMaterials() const { return m_Materials; }
    inline const std::vector<SComponent>& GetComponents() const { return m_Components; }

    // Can be used in Cookers for :
    //   Return as OpenGL VBO - shader rendering
    //   Return as Displaylist - pre-2.0 shaderless rendering

    ModelData(void);
    ~ModelData(void);
};

} // namespace engine
