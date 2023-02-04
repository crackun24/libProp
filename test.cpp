#include "libProp.h"
#include <iostream>
#include <sstream>

using namespace std;

int main()
{
	try {
		libProp::Config conf = libProp::Config::Parse("./Config.properties");
		cout << conf["test"]->as<int>() << endl;
		//getchar();
	}
	catch (const exception& e)
	{
		cout << e.what();
	}
	getchar();
}