#include  <iostream>
#include <string>
#include <sstream>

#include "vector-impl.h"

using namespace boost::interprocess;

VectorImpl::VectorImpl(std::string pathToFile)
	:Implementation(pathToFile), m_segment(open_or_create, m_sharedMemName.c_str(), 65536)
{
	const ShmemAllocator alloc_inst(m_segment.get_segment_manager());
	m_pmyvector = m_segment.construct<MyVector>("MyVector")(alloc_inst);
}

VectorImpl::~VectorImpl() {
	if (m_segment.find<MyVector>("MyVector").first) {
		m_segment.destroy<MyVector>("MyVector");
	}
}

int VectorImpl::run() {
	

	for (int i = 0; i < 100; ++i)  
		m_pmyvector->push_back(i);



	return 0;
}