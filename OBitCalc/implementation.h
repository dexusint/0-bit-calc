#pragma once

#include <string>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <string>
#include <cstdlib> //std::system

class Implementation
{
public:
	Implementation(std::string pathToFile);
	virtual ~Implementation();
	virtual int run() = 0;
protected:
	std::string m_pathToFile;
	std::string m_sharedMemName;
};
