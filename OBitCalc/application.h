#pragma once

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
	const static int FILE_SIZE = 1024; // 1 KByte
	Application(std::unique_ptr<Implementation> impl):
		m_pImpl(move(impl))
	{}

	void run() {
		m_pImpl->run();
	}

	Application(const Application& rhs) = delete;
	Application& operator=(const Application& rhs) = delete;

	template <class Generator>
	static void createTestFile(int numBytes = FILE_SIZE, Generator gen = []()->char {return '0'; }) {
		ofstream out("test.txt", std::ofstream::out);
		generate_n(std::ostream_iterator<char>(out, ""), FILE_SIZE, gen);
		out.close();
	}



private:
	std::unique_ptr<Implementation> m_pImpl;
};
