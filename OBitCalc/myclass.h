#pragma once
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

class MyClass {
public:
	typedef boost::interprocess::managed_shared_memory::segment_manager                       segment_manager_t;
	typedef boost::interprocess::allocator<void, segment_manager_t>                           void_allocator;
	typedef boost::interprocess::allocator<int, segment_manager_t>                            int_allocator;
	typedef boost::interprocess::vector<int, int_allocator>                                   int_vector;
	typedef boost::interprocess::allocator<int_vector, segment_manager_t>                     int_vector_allocator;

	MyClass(const void_allocator &void_alloc)
		:m_myVector(void_alloc), m_resVector(void_alloc)
	{}

	boost::interprocess::interprocess_mutex      initDataMutex;
	boost::interprocess::interprocess_mutex      processFileMutex;
	boost::interprocess::interprocess_mutex      processedCountMutex;

	boost::interprocess::interprocess_condition  cond_empty;
	boost::interprocess::interprocess_condition  cond_full;

	boost::interprocess::interprocess_condition  cond_dataReady;

	int_vector m_myVector;
	bool m_myVectorEmpty = true;
	int_vector m_resVector;
	unsigned long int m_processedCount = 0;
};
