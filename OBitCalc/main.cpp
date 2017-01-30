#include <iostream>
#include <algorithm>

#include "application.h"
#include "myclassimpl.h"

using namespace std;

int main(int argc, char *argv[])
{

	if (argc == 2) {
		auto gen = []()->char {return '0'; };
		Application::createTestFile<decltype(gen)>(10, gen);

		cout << "File generation comleted" << endl;
	}

	if (argc != 4) {
		cout << "Two parametrs needed (2nd == 1 for vector implementation)" << endl;
		cout << "End of work";

		getchar();
		return 1;
	}
	string programPath(argv[0]);
	size_t pos = programPath.find_last_of("/\\");
	string pathToFile(programPath.substr(0, pos) +'\\' + argv[1]);
	replace(pathToFile.begin(), pathToFile.end(), '\\', '/');
	const int implementationType(atoi(argv[2]));
	const int firstProcess(atoi(argv[3]));
	unique_ptr<Implementation> impl;
	
	switch (implementationType) {
		case(1) : {
			impl.reset(new MyClassImpl(pathToFile, firstProcess));
			break;
		}
		default: {
			cout << "Incorrect implementation" << endl;
			return 1;
		}
	}

	Application app(move(impl));
	app.run();

	getchar();
	return 0;
}