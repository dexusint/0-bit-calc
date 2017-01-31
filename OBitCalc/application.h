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
	const static int DEFAULT_FILE_SIZE = 1024 * 1024; // Bytes
	Application(std::unique_ptr<Implementation> impl):
		m_pImpl(move(impl))
	{}

	void run() {
		m_pImpl->run();
	}

	Application(const Application& rhs) = delete;
	Application& operator=(const Application& rhs) = delete;

	template <class Generator>
	static void createTestFile(const std::string fileName = "test.txt", int numBytes = DEFAULT_FILE_SIZE, Generator gen = []() {return 1; }) {
		ofstream out(fileName, std::ios::out | std::ios::binary);
		std::vector<char> arr(numBytes, 0);
		generate_n(arr.begin(), numBytes, gen);
		out.write((const char *)arr.data(), sizeof(char) * numBytes);
		//std::ostream_iterator<bint> out_iter(out);
		//copy(arr.begin(), arr.end(), out_iter);
		out.close();
	}



private:
	std::unique_ptr<Implementation> m_pImpl;
};

#endif //APPLICATION_H
