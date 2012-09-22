#pragma once
#include "Typedefs.h"
#include <map>

// I've created a struct since it's more flexible
struct SLoadParams
{
	std::map<std::string, std::string> Params;
	SLoadParams() { }
	
	bool GetParam(std::string const& name, std::string& ret) const
	{
		std::map<std::string, std::string>::const_iterator Iter = Params.find(name);
		if (Iter == Params.end())
			return false;
		ret = Iter->second;
		return true;
	}

	explicit SLoadParams (std::string const& simplePath)
	{
		Params["path"] = simplePath;
	}
};

class CResource abstract
{
public:
	virtual std::string Load(SLoadParams const& loadParams) abstract;
	virtual void Unload () abstract;
	// friend class CResManager;
	CResource(void) { }
	virtual ~CResource(void) { }
};
