#pragma once
#include <string>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "implementation.h"

#include "myclass.h"

class MyClassImpl : public Implementation {

boost::interprocess::managed_shared_memory m_segment;
MyClass *m_pMyClass;
std::string m_pathToFile;

public:
	typedef boost::interprocess::managed_shared_memory::segment_manager segment_manager_t;
	typedef boost::interprocess::allocator<void, segment_manager_t> void_allocator;
	MyClassImpl(std::string pathToFile, bool firstProcess):
		m_pathToFile(pathToFile),
		Implementation(pathToFile, firstProcess),
		m_segment(boost::interprocess::open_or_create, m_sharedMemName.c_str(), 65536)
	{
		void_allocator alloc_inst(m_segment.get_segment_manager());
		m_pMyClass = m_segment.find_or_construct<MyClass>("MyClass")(alloc_inst);
	}

	~MyClassImpl() {
		if (m_segment.find<MyClass>("MyClass").first) {
			m_segment.destroy<MyClass>("MyClass");
		}
	}

	int run() override;
	unsigned int const BLOCK_SIZE = 1024 * 1024 * 10;

private:
	int initData();
	void processFileSegment(int segment);
	unsigned long long int index = 0;
};