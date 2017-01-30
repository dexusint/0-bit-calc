#include <fstream>
#include <iostream>
#include <numeric>

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
	unsigned long int length = ifile.tellg();
	ifile.close();

	if (length > 0 && length <= BLOCK_SIZE) {
		m_pMyClass->m_myVector.push_back(0);
		m_pMyClass->m_resVector.push_back(0);
	}
	else if (length > BLOCK_SIZE) {
		m_blocksCount = length%BLOCK_SIZE ? length / BLOCK_SIZE : 1 + length / BLOCK_SIZE;
		m_pMyClass->m_myVector.assign(m_blocksCount, 0);
		m_pMyClass->m_resVector.assign(m_blocksCount, 0);
	}

	m_fileLength = length;

	return 0;
}

void MyClassImpl::processFileSegment(int segment) {

}

int MyClassImpl::run() {
	fstream ifile(m_pathToFile, std::ofstream::in | std::ofstream::binary);
	if (!ifile.is_open()) {
		cout << "Error opening file" << endl;
		ifile.close();
		return 1;
	}

	char *pChars = new char[BLOCK_SIZE];
	
	{
		scoped_lock<interprocess_mutex> lock(m_pMyClass->initDataMutex, try_to_lock);
		if (lock && m_pMyClass->m_myVectorEmpty) {
			initData();
			m_pMyClass->m_myVectorEmpty = false;
		}
	}

	while (true) {
		unsigned long int index = 0;
		{
			scoped_lock<interprocess_mutex> lock(m_pMyClass->processFileMutex);
			if (!m_pMyClass->m_myVectorFree) {
				m_pMyClass->cond_waitMyVector.wait(lock);
			}
			const auto pos = find(m_pMyClass->m_myVector.begin(), m_pMyClass->m_myVector.end(), 0);

			if (pos != m_pMyClass->m_myVector.end()) {
				index = distance(m_pMyClass->m_myVector.begin(), pos);
				m_pMyClass->m_myVector[index] = 1;
				m_pMyClass->m_myVectorFree = true;
				m_pMyClass->cond_waitMyVector.notify_one();
			}
			else {
				lock.unlock();
				{
					scoped_lock<interprocess_mutex> lockDataOutput(m_pMyClass->processedCountMutex);

					if (m_pMyClass->m_processedCount < m_blocksCount) {
						m_pMyClass->cond_dataReady.wait(lockDataOutput);
					}
					
					unsigned long int result = accumulate(m_pMyClass->m_resVector.begin(), m_pMyClass->m_resVector.end(), 0);
					cout << "Num of zero bits is: " << result << endl;
				}

				m_pMyClass->cond_dataReady.notify_all();

				break;
			}
		}

		
		ifile.seekg(index * BLOCK_SIZE, std::ios_base::beg);
		unsigned long int length = m_fileLength - index * BLOCK_SIZE;
		if (length > BLOCK_SIZE) length = BLOCK_SIZE;
		
		ifile.read(pChars, length);

		unsigned long int sum = accumulate(pChars, pChars + length, 0, [](unsigned long int init, char b) {
			unsigned long int sum = init;
			for (char bit = 0; bit < 8; ++bit) {
				sum += (1 << bit) & b ? 0 : 1;
			}
			return sum;
		});

		m_pMyClass->m_resVector[index] = sum;
		{
			scoped_lock<interprocess_mutex> lock(m_pMyClass->processedCountMutex);
			m_pMyClass->m_processedCount++;
		}

		
	}

	delete[] pChars;

	ifile.close();

	return 0;
}