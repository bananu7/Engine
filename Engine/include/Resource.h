#pragma once
#include <map>
#include <boost/optional.hpp>
#include <iostream>
#include <vector>
#include <iterator>

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
 
std::vector<unsigned char> buffer_from_file(std::istream& stream) {
    typedef std::istream::pos_type pos_type; 
    typedef std::istream::off_type off_type;
    typedef std::istream_iterator<char> i_iter;
 
    std::vector<unsigned char> ret;
    stream >> std::noskipws;
    pos_type pre = stream.tellg();
    if (stream.seekg(0, std::ios_base::end)) {
        ret.resize(stream.tellg());
        stream.seekg(pre, std::ios_base::beg);
    }
    std::copy(i_iter(stream), i_iter(), std::back_inserter(ret));
    return ret;
}

class CResource abstract
{
public:
	virtual std::string Load(ILoader const& loadParams) abstract;
	virtual void Unload () abstract;
	// friend class CResManager;
	CResource(void) { }
	virtual ~CResource(void) { }
};
