#include <fstream>
#include <iostream>
#include <numeric>

//#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/thread/thread.hpp> 

#include "myclassimpl.h"

using namespace std;
using namespace boost::interprocess;

int MyClassImpl::initData() {
	fstream ifile(m_pathToFile, std::ofstream::in | std::ofstream::binary);
	if (!ifile.is_open()) {
		cout << "Error opening file" << endl;
		ifile.close();
		return 1;
	}
	ifile.seekg(0, std::ios_base::end);
	unsigned long long int length = ifile.tellg();
	ifile.close();

	while (length / BLOCK_SIZE > 1000)
	{
		BLOCK_SIZE *= 2;
	}

	if (length > 0 && length <= BLOCK_SIZE) {
		m_pMyClass->m_blocksCount = 1;
		m_pMyClass->m_myVector.push_back(0);
		m_pMyClass->m_resVector.push_back(0);
	}
	else if (length > BLOCK_SIZE) {
		m_pMyClass->m_blocksCount = length%BLOCK_SIZE ? length / BLOCK_SIZE : 1 + length / BLOCK_SIZE;
		m_pMyClass->m_myVector.assign(m_pMyClass->m_blocksCount, 0);
		m_pMyClass->m_resVector.assign(m_pMyClass->m_blocksCount, 0);
	}

	m_pMyClass->m_fileLength = length;
	m_pMyClass->BLOCK_SIZE = BLOCK_SIZE;

	return 0;
}

int MyClassImpl::run() {
	fstream ifile(m_pathToFile, std::ofstream::in | std::ofstream::binary);
	if (!ifile.is_open()) {
		cout << "Error opening file" << endl;
		ifile.close();
		return 1;
	}

	{
		scoped_lock<interprocess_mutex> lock(m_pMyClass->initDataMutex);
		if (m_pMyClass->m_myVectorEmpty) {
			initData();
			m_pMyClass->m_myVectorEmpty = false;
		}
	}
	BLOCK_SIZE = m_pMyClass->BLOCK_SIZE;
	unique_ptr<char[]> pChars(new char[BLOCK_SIZE]);
	char* const raw_pChars = pChars.get();

	while (true) {
		
		{
			scoped_lock<interprocess_mutex> lock(m_pMyClass->processFileMutex);
			if (!m_pMyClass->m_myVectorFree) {
				m_pMyClass->cond_waitMyVector.wait(lock);
			}

			if (m_pMyClass->index < m_pMyClass->m_blocksCount) {
				m_pMyClass->m_myVectorFree = true;
				index = m_pMyClass->index++;
				cout << m_pMyClass->index << endl;
				lock.unlock();
				m_pMyClass->cond_waitMyVector.notify_one();
			}
			else {
				lock.unlock();
				{
					scoped_lock<interprocess_mutex> lockDataOutput(m_pMyClass->processedCountMutex);
					if (m_pMyClass->m_processedCount != m_pMyClass->m_blocksCount) {
						m_pMyClass->cond_dataReady.wait(lockDataOutput);
					}
					else {
						m_pMyClass->cond_dataReady.notify_all();
					}
					
					unsigned long long int result = accumulate(m_pMyClass->m_resVector.begin(), m_pMyClass->m_resVector.end(), (unsigned long long int)0);
					cout << "Num of zero bits is: " << result << endl;
				}

				break;
			}
		}

		
		ifile.seekg(index * BLOCK_SIZE, std::ios_base::beg);
		unsigned long long int length = m_pMyClass->m_fileLength - index * BLOCK_SIZE;
		if (length > BLOCK_SIZE) length = BLOCK_SIZE;
		
		
		ifile.read(raw_pChars, length);

		unsigned long long int sum = accumulate(raw_pChars, raw_pChars + length, (unsigned long long int)0, [](unsigned long long int init, char b) {
			unsigned long long int sum = init;
			for (char bit = 0; bit < 8; ++bit) {
				if(((1 << bit) & b) == 0)sum++;
			}
			return sum;
		});

		m_pMyClass->m_resVector[index] = sum;
		{
			scoped_lock<interprocess_mutex> lock(m_pMyClass->processedCountMutex);
			m_pMyClass->m_processedCount++;
		}


		//boost::this_thread::sleep(boost::posix_time::milliseconds(100));

		
	}

	ifile.close();

	return 0;
}