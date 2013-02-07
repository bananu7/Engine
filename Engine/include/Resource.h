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
	virtual std::unique_ptr<std::istream> GetDataStream(std::string const& name) const = 0;
	virtual boost::optional<std::vector<unsigned char>> GetRawData(std::string const& name) const = 0;
};

class CSimpleFileLoader :
	public ILoader
{
	std::map<std::string, std::string> Params;

public:
	boost::optional<std::string const&> GetParam(std::string const& name) const override;
	std::unique_ptr<std::istream> GetDataStream(std::string const& name) const override;
	boost::optional<std::vector<unsigned char>> GetRawData(std::string const& name) const override;
	explicit CSimpleFileLoader::CSimpleFileLoader (std::string const& simplePath);
	explicit CSimpleFileLoader::CSimpleFileLoader (std::map<std::string, std::string> const& data);
};

class CSimpleDirectLoader :
	public ILoader
{
public:
	using TDataVal = std::vector<unsigned char>;
	using TDataMap = std::map<std::string, TDataVal>;

private:
	TDataMap Data;

public:
	boost::optional<std::string const&> GetParam(std::string const& name) const override;
	std::unique_ptr<std::istream> GetDataStream(std::string const& name) const override;
	boost::optional<std::vector<unsigned char>> GetRawData(std::string const& name) const override;

	explicit CSimpleDirectLoader (TDataMap const& data);
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
