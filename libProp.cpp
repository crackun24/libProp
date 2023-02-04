#include "libProp.h"

using namespace std;
using namespace libProp;

Value::~Value()
{
	if (this->mType == ValType::ValueType)//为数据类型
		delete this->mVal;//释放数据资源
	else if (this->mType == ValType::ArrayType)
		delete this->mArray;
}

void libProp::Config::ParseLineStr(std::string& data)
{
	if (data.find("=") == string::npos)//判断是否为有效的行
		return;//不是有效行返回
}


Config Config::Parse(std::string& filePath)
{
	try {
		fstream fs(filePath);
		if (fs.is_open())
			throw runtime_error("Could not open file.");

		string temp;
		Config conf;//配置文件对象
		while (getline(fs, temp))//一行行读取配置文件信息
		{
			conf.ParseLineStr(temp);//解析单行数据
		}
	}
	catch (const exception& e)
	{
		throw string("libProp:An error occured when parsing the file,error message: ") + e.what();
	}
}

Value* Config::operator[](const std::string& key)
{
	shared_lock<shared_mutex>lg(this->mLocker);
	return this->mConfMap[key];
}
