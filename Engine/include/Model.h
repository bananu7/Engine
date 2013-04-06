#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include "VertexBuffer.h"

namespace engine {

class ModelData;
class Program;

// It's constructed from ModelData
// It's the highest abstraction on the displayed object

class CNode 
{
public:
    glm::vec3 m_Rotation, m_Position, m_Scale;
};

class Model :
    protected CNode
{
protected:
    unsigned int m_DefaultGroup;
    boost::ptr_vector<VertexBuffer> m_Vbos;

public:
    std::shared_ptr<Program> Shader;

    void Draw () const;
    void Draw (glm::vec3 const& pos, glm::vec3 const& rot, glm::vec3 const& scale) const;

    Model(ModelData const& data);
};

} // namespace engine
