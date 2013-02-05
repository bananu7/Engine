#pragma once
#include <map>
#include <exception>
#include <memory>
#include "Resource.h"
#include "Logger.h"

template <typename T> class CResPtr;

using std::string;

typedef std::map<std::string, std::shared_ptr<CResource>> TResMap;
typedef std::map<std::string, std::unique_ptr<ILoader>> TCatalogMap;

class CResourceSupply
{
protected:
	TResMap		m_Data;
	TCatalogMap m_Catalog;
	std::string	m_ResourcePath;
	std::string m_ConfigPath;

public:
	template <typename T>
	T* GetResource (const std::string& ident) throw() try
	{
		// Najpierw sprawdzamy, czy mo¿e mamy ju¿ w pamiêci taki zasób
		TResMap::iterator It = m_Data.find(ident);
		if (It != m_Data.end())
			// Jeœli tak, to po prostu go zwracamy
			// Upewniaj¹c siê, ¿e ma w³aœciwy typ
			//return dynamic_cast<T*>((*It).second); 
			return dynamic_cast<T*>(It->second.get());
		else
		{
			// Jeœli nie, sprawdzamy, czy wiemy sk¹d go za³adowaæ
			TCatalogMap::iterator CatIt = m_Catalog.find(ident);
			if (CatIt != m_Catalog.end())
			{
				// Jeœli tak, tworzymy sobie nowy obiekt
				T* Temp = new T();
				// £adujemy go
				// TODO: kilka roznych sciezek z zasobami
				string Result = Temp->Load ((*CatIt).second);
				if (Result == "")
				{
					// Dopisujemy go do listy za³adowanych zasobów
					boost::shared_ptr<CResource> TempC (Temp);
					m_Data.insert (std::make_pair(ident, TempC));
					// I zwracamy, bez dynamic_cast, bo ma ju¿ dobry typ
					return static_cast<T*>(TempC.get());
				}
				else 
				{
					// £adowanie siê nie powiod³o
					
					throw EXCEPTION ("Resource Manager", "Loading of asset "+ident+" failed : "+Result);
					return 0;
				}
			}
			else
			{
				// Nie mamy zasobu w katalogu
				throw EXCEPTION ("Resource Manager", "Unknown asset "+ident);
				return 0;
			}
		}
	}
	catch (CException& e)
	{
		LOG(e);
		_CrtDbgBreak();
		return 0;
	}
	inline const std::string& GetResourcePath () const { return m_ResourcePath; }

	bool LoadCatalog (const std::string& path);
	// Zaladuj wszystko z podanego folderu, jesli sciezka pusta, zaladuj resource folder
	bool LoadWholeFolder(const std::string& path);
	inline void AddCatalogEntry (const std::string& ident, std::unique_ptr<ILoader> loader)
	{
		if (m_Catalog.find(ident) == m_Catalog.end())
			m_Catalog.insert(std::make_pair(ident, loader));
	}
};

