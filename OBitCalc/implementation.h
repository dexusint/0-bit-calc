#pragma once

#include <string>

class Implementation
{
public:
	Implementation(std::string pathToFile)
		:m_pathToFile(pathToFile)
	{}
	virtual int run() = 0;
protected:
	std::string m_pathToFile;
};
