#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include "VertexBuffer.h"

class CModelData;
class CShader;

// It's constructed from CModelData
// It's the highest abstraction on the displayed object

class CNode 
{
public:
	glm::vec3 m_Rotation, m_Position, m_Scale;
};

class CModel :
	protected CNode
{
protected:
	unsigned int m_DefaultGroup;
	boost::ptr_vector<CVertexBuffer> m_Vbos;

public:
	std::shared_ptr<CShader> Shader;

	void Draw () const;
	void Draw (const glm::vec3 const& pos, glm::vec3 const& rot, glm::vec3 const& scale) const;

	CModel(CModelData const& data);
};

