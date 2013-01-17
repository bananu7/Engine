
#include "Resource.h"
#include <fstream>

using namespace std;

boost::optional<std::string const&> CSimpleFileLoader::GetParam(std::string const& name) const
{
	auto Iter = Params.find(name);
	if (Iter == Params.end())
		return boost::none;
	return boost::optional<std::string const&>(Iter->second);
}

boost::optional<std::istream> CSimpleFileLoader::GetDataStream(std::string const& name)
{
	auto Iter = Params.find(name);
	if (Iter == Params.end())
		return boost::none;

	ifstream FileStream (Iter->second);
	if (FileStream.bad())
		return boost::none;

	return boost::optional<std::istream>(std::move(FileStream));
}

