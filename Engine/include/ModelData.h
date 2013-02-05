#pragma once
#include <vector>
#include <string>
#include <map>
#include <glm/glm.hpp>

class CModelData
{
public:
// TODO - zamienic na definicje cross-compiler
#pragma pack (show)
#pragma pack (push, 1) 
	struct SVector3 { float X, Y, Z; SVector3() { } };
	struct SVector2 { float U, V;  };

	struct SFace
	{
		unsigned int VertIndices[3]; // Obslugujemy tylko modele triangulizowane
		unsigned int NormIndices[3];
		unsigned int TexCIndices[3];
	};
#pragma pack (pop)
	struct SMaterial 
	{
		float Specular[4], Ambient[4], Diffuse[4]; 
		std::vector<int> Textures;
		SMaterial() { }
	};

	struct SGroup
	{
		std::string MaterialName;
		std::vector<SFace> Faces;

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
	std::vector<SVector3> m_Vertices;
	std::vector<SVector3> m_Normals;
	std::vector<SVector2> m_TexCoords;
	std::map<std::string, SMaterial> m_Materials;
	std::vector<SComponent> m_Components;

	bool CModelData::_LoadFromFBX(const std::string &path);
	bool _LoadFromObj (std::istream& str);
	bool _LoadMtl (std::istream& str);

public:
	inline const std::vector<SVector3>& GetVertices() const { return m_Vertices; }
	inline const std::vector<SVector3>& GetNormals() const { return m_Normals; }
	inline const std::vector<SVector2>& GetTexCoords() const { return m_TexCoords; }
	inline const std::map<std::string, SMaterial>& GetMaterials() const { return m_Materials; }
	inline const std::vector<SComponent>& GetComponents() const { return m_Components; }

	bool Load (std::string const& path);
	// Can be used in Cookers for :
	//   Return as OpenGL VBO - shader rendering
	//   Return as Displaylist - pre-2.0 shaderless rendering

	CModelData(void);
	~CModelData(void);
};

