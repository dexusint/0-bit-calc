#include <iostream>
#include <algorithm>

#include "application.h"
#include "myclassimpl.h"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc == 4) {
		string fileName(argv[1]);
		const int fileSize(atoi(argv[2])); //In Mbytes
		const char value((unsigned char)atoi(argv[3]));
		auto gen = [value]() {return value; };
		Application::createTestFile<decltype(gen)>(fileName, fileSize * 1024 * 1024, gen);

		cout << "File generation comleted" << endl;
	}else if (argc == 3) {
		string programPath(argv[0]);
		size_t pos = programPath.find_last_of("/\\");
		string pathToFile(programPath.substr(0, pos) + '\\' + argv[1]);
		replace(pathToFile.begin(), pathToFile.end(), '\\', '/');
		const bool firstProcess(atoi(argv[2]) ? true : false);
		unique_ptr<Implementation> impl;
		impl.reset(new MyClassImpl(pathToFile, firstProcess));

		Application app(move(impl));
		app.run();

		getchar();
	}
	else {
		cout << "For file generation print pass next params fileName, size of file (Mbytes) and value [0, 256)" << endl;
		cout << "Example:  ./OBitCalc.exe \"text.txt\" \"100\" \"255\"" << endl;
		cout << endl;
		cout << "For file generation print pass next params fileName, master(1)/slave(0) (master should be runned first)" << endl;
		cout << "Example:  ./OBitCalc.exe \"text.txt\" \"1\"" << endl;
	}

	return 0;
}