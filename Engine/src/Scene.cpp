
#include "Scene.h"
#include "ResManager.h"
#include "ModelFbx.h"
#include "Misc.h"
#include "Image.h"
#include "Shader.h"
#include "ShaderSet.h"
#include <rapidxml.hpp>
#include <fstream>

using namespace std;
using rapidxml::xml_node;

void CScene::SSceneObject::Draw () const
{
	Shader->Bind();
	for(int it= 0, itE=Textures.size(); it!=itE; ++it)
	{
		CResManager::GetSingleton()->GetResource<CImage>(Textures[it])->Bind(it);
		Shader->SetTex("Texture"+boost::lexical_cast<string>(it),it);
	}
	Model->Draw(Position, Rotation, Scale);
}

bool CScene::LoadFromFile (const std::string& path)
{
	uint32 ObjCount;
	string ModelName;
	SSceneObject Temp;

	rapidxml::xml_document<> Doc;

	char* FileData = TextFileRead(path.c_str());
	if (FileData == NULL)
		return false;

	Doc.parse<0>(FileData);
	auto Root = Doc.first_node();

	for (xml_node<>* node = Root->first_node("object"); node != NULL; node = node->next_sibling("object"))
	{
		Temp.Position = CVector3::GetZero();
		Temp.Rotation = CVector3::GetZero();
		Temp.Scale = CVector3(1.f, 1.f, 1.f);

		ModelName = string(node->first_attribute("type")->value());

		xml_node<>* PositionNode = node->first_node("position");
		if (PositionNode)
		{
			Temp.Position.X = atof(PositionNode->first_attribute("x")->value());
			Temp.Position.Y = atof(PositionNode->first_attribute("y")->value());
			Temp.Position.Z = atof(PositionNode->first_attribute("z")->value());
		}
		xml_node<>* RotationNode = node->first_node("rotation");
		if (RotationNode)
		{
			Temp.Rotation.X = atof(RotationNode->first_attribute("x")->value());
			Temp.Rotation.Y = atof(RotationNode->first_attribute("y")->value());
			Temp.Rotation.Z = atof(RotationNode->first_attribute("z")->value());
		}
		xml_node<>* ScaleNode = node->first_node("scale");
		if (ScaleNode)
		{
			Temp.Scale.X = atof(ScaleNode->first_attribute("x")->value());
			Temp.Scale.Y = atof(ScaleNode->first_attribute("y")->value());
			Temp.Scale.Z = atof(ScaleNode->first_attribute("z")->value());
		}
		Temp.Model = CResManager::GetSingleton()->GetResource<CModelFbx>(ModelName);

		xml_node<>* ShaderNode = node->first_node("shader");
		if (ShaderNode)
		{
			string ShaderName = ShaderNode->value();
			Temp.Shader = CResManager::GetSingleton()->GetResource<CShader>(ShaderName);
			Temp.Model->m_Shader = CResManager::GetSingleton()->GetResource<CShader>(ShaderName);
		}
		else
		{
			Temp.Shader = CResManager::GetSingleton()->GetResource<CShader>("shader_light");
			Temp.Model->m_Shader = CResManager::GetSingleton()->GetResource<CShader>("shader_light");
		}

		xml_node<>* TextureNode = node->first_node("texture");
		if (TextureNode)
		{
			Temp.Textures.clear();
			int TextureCount= atoi(TextureNode->first_attribute("count")->value());
			xml_node<>* TextureInfoNode= TextureNode->first_node("name");
			for(int it= 0, itE= TextureCount; it!=itE; ++it)
			{
				Temp.Textures.push_back(TextureInfoNode->value());
				TextureInfoNode = TextureInfoNode->next_sibling("name");			
			}
		}
		if (Temp.Position.Y < 0.f)
			Temp.Position.Y = m_GroundSampler(Temp.Position.X, Temp.Position.Z);
	
		m_Objects.insert(new SSceneObject(Temp));
	}
}

void CScene::Draw ()
{
	for (auto It = m_Objects.begin(); It != m_Objects.end(); It++)
	{
		(*It)->Draw();
	}
}

CScene::CScene(void)
{
}


CScene::~CScene(void)
{
	for (auto It = m_Objects.begin(); It != m_Objects.end(); It++)
	{
		delete (*It);
	}
}
