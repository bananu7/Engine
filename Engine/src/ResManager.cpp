#include "ResManager.h"
#include "Resource.h"
#include "Misc.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

#include <pugixml.hpp>
#include <exception>

using namespace std;
using pugi::xml_document;
using pugi::xml_node;
using pugi::xml_attribute;

/*bool CResManager::LoadCatalog (const string& path)
{
	// Config nie powinien byc w resource pathie!
	std::fstream File (m_ConfigPath + path, std::ios::in);
	if (!File.is_open())
		return false;
	while (!File.eof())
	{
		string ResourceName, ResourcePath;
		File >> ResourceName >> ResourcePath;
		if (!ResourceName.empty() && !ResourcePath.empty())
			AddCatalogEntry (ResourceName, ResourcePath);
		else
			return false;
	}
	return true;
}*/

bool CResourceSupply::LoadCatalog(string const& path)
{
	xml_document Doc;
	if (Doc.load_file(path.c_str()).status != pugi::status_ok)
		return false;

	xml_node Root = Doc.first_child();
	for (xml_node Resource = Root.child("res"); Resource; Resource = Resource.next_sibling("res"))
	{
		string Name = Resource.attribute("name").as_string();
		string Type = Resource.attribute("type").as_string();

		// FIXME
		//ILoader LoadParams;

		for (xml_node LoadParam = Resource.first_child(); LoadParam; LoadParam = LoadParam.next_sibling())
		{
			string ParamName = LoadParam.name();
			
			// Loadpasses is a collection of load params
			//TODO
			//if (ParamName != "load_passes")
			//{
				// child value, bo to, co jest w srodku node'a to tzw. node_pcdata
				string ParamValue = LoadParam.child_value();

			//	LoadParams.Params[ParamName] = ParamValue;
			//}
		}
		//m_Catalog[Name] = LoadParams;
	}
	// FIXME
	return false;
}

bool CResourceSupply::LoadWholeFolder(const std::string& path)
{
	boost::filesystem::path LoadPath(m_ResourcePath+path);

	if (!boost::filesystem::exists(LoadPath) || !boost::filesystem::is_directory(LoadPath))
	{
		throw std::exception("ResourceManager : LoadWholeFolder - can't load folder: doesn't exist or isn't directory");
		return false; // Is not directory or doesnt exist
	}

	boost::filesystem::directory_iterator It(LoadPath),EndIterator;
	while (It!=EndIterator)
	{
		// A dir inside dir, load those too, if doesn't have a dot in front of filename - they are hidden, no use for us
		if (boost::filesystem::is_directory(*It))
		{	
			if (It->path().filename().string()[0]!='.')
			{
				std::string TempFolder=It->path().string();
				// Musimy usunac resourcepath bo bedzie dodany potem
				TempFolder=TempFolder.substr(m_ResourcePath.size(),TempFolder.size()-m_ResourcePath.size());
				LoadWholeFolder(TempFolder);
			}
		} else
		{
			// Wsadzamy plik do mapy
			std::string ResourceName=boost::filesystem::basename(It->path().leaf());
			std::string ResourcePath=(It->path().string());
			// Usuwamy resourcepath
			ResourcePath=ResourcePath.substr(m_ResourcePath.size(),ResourcePath.size()-m_ResourcePath.size());
			// FIXME
		//	AddCatalogEntry(ResourceName,ILoader(ResourcePath));
		}
		It++;
	}
	return true;

}

CResourceSupply::CResourceSupply()
{
	m_ResourcePath = "../Resources/";
	m_ConfigPath = "../Resources/";
}

