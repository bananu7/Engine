#pragma once
#include <string>

class CException
{
public:
	std::string FullDesc;
	std::string Component;
	std::string File;
	int			Line;

	CException(const std::string& component, const std::string& fullDesc, const std::string& file, int line) :
		FullDesc (fullDesc),
		Component (component),
		File (file),
		Line (line)
	{ }
};

#define EXCEPTION(X,Y) CException(X,Y,__FILE__,__LINE__)
