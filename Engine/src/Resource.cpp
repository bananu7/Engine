#include "Resource.h"
#include <istream>
#include <fstream>
#include <string>

using namespace std;

std::vector<unsigned char> buffer_from_file(std::istream& stream)
{
    typedef std::istream::pos_type pos_type; 
    typedef std::istream::off_type off_type;
    typedef std::istream_iterator<unsigned char> i_iter;
 
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

CSimpleFileLoader::CSimpleFileLoader (std::map<std::string, std::string> const& data)
{
	Params.insert(data.begin(), data.end());
}
CSimpleFileLoader::CSimpleFileLoader (std::string const& simplePath)
{
	Params["path"] = simplePath;
}

boost::optional<std::string const&> CSimpleFileLoader::GetParam(std::string const& name) const
{
	auto Iter = Params.find(name);
	if (Iter == Params.end())
		return boost::none;
	return boost::optional<std::string const&>(Iter->second);
}

std::unique_ptr<std::istream> CSimpleFileLoader::GetDataStream(std::string const& name) const
{
	auto Iter = Params.find(name);
	if (Iter == Params.end())
		return unique_ptr<ifstream>();

	std::unique_ptr<ifstream> FileStream (new ifstream(Iter->second));
	if (FileStream->bad())
		return unique_ptr<ifstream>();

	return std::move(FileStream);
}

boost::optional<std::vector<unsigned char>> CSimpleFileLoader::GetRawData(std::string const& name) const
{
	auto Iter = Params.find(name);
	if (Iter == Params.end())
		return boost::none;

	ifstream FileStream(Iter->second.c_str(), ios::binary);
	if (!FileStream)
		return boost::none;

	vector<unsigned char> Temp(
        (std::istreambuf_iterator<char>(FileStream)),
        std::istreambuf_iterator<char>()
		);

	return boost::optional<std::vector<unsigned char>>(Temp);
}

CSimpleDirectLoader::CSimpleDirectLoader (TDataMap const& data)
{
	Data.insert(data.begin(), data.end());
}

boost::optional<std::string const&> CSimpleDirectLoader::GetParam(std::string const& name) const
{
	//FIXME - should simple, direct loader be able to handle params? I think yes
	return boost::none;
}

std::unique_ptr<std::istream> CSimpleDirectLoader::GetDataStream(std::string const& name) const
{
	// FIXME
	return std::move(unique_ptr<istream>(new ifstream()));
}

boost::optional<std::vector<unsigned char>> CSimpleDirectLoader::GetRawData(std::string const& name) const
{
	auto Iter = Data.find(name);
	if (Iter == Data.end())
		return boost::none;

	return boost::optional<std::vector<unsigned char>>(Iter->second);
}
