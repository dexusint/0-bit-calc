#pragma once

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include "implementation.h"

class VectorImpl : public Implementation {
public:
	typedef boost::interprocess::allocator<int, boost::interprocess::managed_shared_memory::segment_manager>  ShmemAllocator;
	typedef boost::interprocess::vector<int, ShmemAllocator> MyVector;
public:
	VectorImpl(std::string pathToFile);
	~VectorImpl();
	int run();

private:
	boost::interprocess::managed_shared_memory m_segment;
	MyVector *m_pmyvector;
};