#ifndef APPLICATION_H
#define APPLICATION_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <memory>

#include "implementation.h"

class Application {
public:
	const static int FILE_SIZE = 1000 * 1024 * 1024; // 1 KByte
	Application(std::unique_ptr<Implementation> impl):
		m_pImpl(move(impl))
	{}

	void run() {
		m_pImpl->run();
	}

	Application(const Application& rhs) = delete;
	Application& operator=(const Application& rhs) = delete;

	template <class Generator>
	static void createTestFile(int numBytes = FILE_SIZE, Generator gen = []() {return 0; }) {
		ofstream out("test.txt", std::ios::out | std::ios::binary);
		std::vector<char> arr(FILE_SIZE, 0);
		out.write((const char *)arr.data(), sizeof(char) * FILE_SIZE);
		//generate_n(arr, FILE_SIZE, gen);
		//std::ostream_iterator<bint> out_iter(out);
		//copy(arr.begin(), arr.end(), out_iter);
		out.close();
	}



private:
	std::unique_ptr<Implementation> m_pImpl;
};

#endif //APPLICATION_H
