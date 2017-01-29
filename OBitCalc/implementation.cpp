#include <iostream>
#include <boost/functional/hash.hpp>

#include "implementation.h"

using namespace boost::interprocess;

std::string ToString(size_t sz) {
	std::stringstream ss;
	ss << sz;
	return ss.str();
}

Implementation::Implementation(std::string pathToFile)
:m_pathToFile(pathToFile)
{
	m_sharedMemName = "HappyMem13";
	//shared_memory_object::remove(m_sharedMemName.c_str());
	//boost::hash<std::string> string_hash;
	//std::size_t hash = string_hash(m_pathToFile);
	//m_sharedMemName = m_pathToFile + ToString(hash);
	//std::cout << hash << std::endl;

}

Implementation::~Implementation() {
	//shared_memory_object::remove(m_sharedMemName.c_str());
}