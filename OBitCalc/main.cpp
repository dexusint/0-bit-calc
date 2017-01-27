#include <iostream>

#include "application.h"
#include "master-impl.h"
#include "slave-impl.h"

using namespace std;

int main(int argc, char *argv[])
{

	if (argc == 2) {
		auto gen = []()->char {return '0'; };
		Application::createTestFile<decltype(gen)>(10, gen);

		cout << "File generation comleted" << endl;
	}

	if (argc != 3) {
		cout << "Two parameters should be passed to application: full path to file and boolean indicator (1 - master, 0 - slave)" << endl;
		cout << "End of work";

		getchar();
		return 1;
	}

	string pathToFile(argv[1]);
	const bool isMaster(atoi(argv[2]));
	unique_ptr<Implementation> impl;
	if (isMaster) {
		impl.reset(new MasterImpl(pathToFile));
	}
	else {
		impl.reset(new SlaveImpl(pathToFile));
	}

	Application app(move(impl));
	app.run();

	getchar();
	return 0;
}