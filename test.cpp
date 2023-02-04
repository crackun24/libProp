#include "libProp.h"
#include <iostream>
#include <sstream>

using namespace std;

int main()
{
	try {
		libProp::Config conf = libProp::Config::Parse("./Config.properties");
	}
	catch (const exception& e)
	{
		cout << e.what();
	}
	getchar();
}