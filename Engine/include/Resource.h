#pragma once
#include <map>
#include <boost/optional.hpp>
#include <memory>
#include <iostream>
#include <vector>
#include <iterator>
#include <string>

class ILoader
{	
public:
	virtual boost::optional<std::string const&> GetParam(std::string const& name) const = 0;
	virtual std::unique_ptr<std::istream> GetDataStream(std::string const& name) = 0;
};

class CSimpleFileLoader :
	public ILoader
{
	std::map<std::string, std::string> Params;

public:
	boost::optional<std::string const&> GetParam(std::string const& name) const override;
	std::unique_ptr<std::istream> GetDataStream(std::string const& name) override;
	explicit CSimpleFileLoader::CSimpleFileLoader (std::string const& simplePath);
	explicit CSimpleFileLoader::CSimpleFileLoader (std::map<std::string, std::string> const& data);
};
 
std::vector<unsigned char> buffer_from_file(std::istream& stream);

class CResource abstract
{
public:
	virtual std::string Load(ILoader const& loadParams) abstract;
	virtual void Unload () abstract;
	// friend class CResManager;
	CResource(void) { }
	virtual ~CResource(void) { }
};
