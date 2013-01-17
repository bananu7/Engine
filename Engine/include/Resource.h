#pragma once
#include "Typedefs.h"
#include <map>
#include <boost/optional.hpp>

class ILoader
{	
public:
	virtual boost::optional<std::string const&> GetParam(std::string const& name) const = 0;
	virtual boost::optional<std::istream> GetDataStream(std::string const& name) = 0;
};

class CSimpleFileLoader :
	public ILoader
{
	std::map<std::string, std::string> Params;

public:
	boost::optional<std::string const&> GetParam(std::string const& name) const override;
	boost::optional<std::istream> GetDataStream(std::string const& name) override;

	explicit CSimpleFileLoader (std::string const& simplePath)
	{
		Params["path"] = simplePath;
	}
};

class CResource abstract
{
public:
	virtual std::string Load(ILoader const& loadParams) abstract;
	virtual void Unload () abstract;
	// friend class CResManager;
	CResource(void) { }
	virtual ~CResource(void) { }
};
