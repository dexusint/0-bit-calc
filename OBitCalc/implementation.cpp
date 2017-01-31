#include <iostream>
#include <boost/functional/hash.hpp>
#include <boost/filesystem/operations.hpp>
#include <ctime>

#include "implementation.h"

using namespace boost::interprocess;

std::string ToString(size_t sz) {
	std::stringstream ss;
	ss << sz;
	return ss.str();
}

Implementation::Implementation(std::string pathToFile, bool firstProcess)
:m_pathToFile(pathToFile)
{
	boost::filesystem::path p(pathToFile);
	std::time_t t = boost::filesystem::last_write_time(p);
	size_t pos = pathToFile.find_last_of("/\\");
	std::string fileName(pathToFile.substr(pos + 1));

	//boost::hash<std::string> string_hash;
	//std::size_t hash = string_hash(m_pathToFile);
	m_sharedMemName = fileName + ToString(t);
	std::cout << m_sharedMemName << std::endl;

	if(firstProcess)
		shared_memory_object::remove(m_sharedMemName.c_str());
}

Implementation::~Implementation() {
	shared_memory_object::remove(m_sharedMemName.c_str());
}