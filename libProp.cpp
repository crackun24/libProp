#include "libProp.h"

using namespace std;
using namespace libProp;

Value::Value(std::string* data)
{
	this->mType = ValueType;
	this->mVal = data;
}

Value::Value(std::vector<std::string>* array)
{
	this->mType = ArrayType;//设置为数组的类型
	this->mArray = array;
}

Value::~Value()
{
	if (this->mType == ValType::ValueType)//为数据类型
		delete this->mVal;//释放数据资源
	else if (this->mType == ValType::ArrayType)
		delete this->mArray;
}

void Config::ParseLineStr(std::string& data)
{
	if (data == "" || data.find("=") == string::npos)//判断是否为有效的行
		return;//不是有效行返回
	string& keyData = data.substr(0, data.find("="));//获取等号前面的数据
	string& valData = data.substr(data.find("=") + 1);//获取等号后面的数据
	EraseFBSpace(keyData);//去除前面的空格
	EraseFBSpace(valData);//去除后面的空格

	if (*valData.begin() == '[')//判断是否以数组的格式开头
	{
		valData.erase(0, 1);
		valData.pop_back();//删除末尾元素

		vector<string>* temp = new vector<string>;
		while (valData != "")//没有提取完毕则一直循环提取
		{
			auto pos = valData.find(",");//获取分隔符的位置
			string tempVal;
			if (pos == string::npos)//判断是否有分隔符
			{
				tempVal = valData;
				valData.clear();//没有分隔符则为最后一个元素
			}
			else {
				tempVal = valData.substr(0, pos);
				valData.erase(0, pos + 1);//删除包括分隔符的的已解析元素
			}
			EraseFBSpace(tempVal);//删除前后的空格
			temp->push_back(tempVal);
		}

		Value* val = new Value(temp);
		this->mConfMap.insert(make_pair(keyData, val));//插入键值映射关系
	}
	else {
		Value* temp = new Value(new string(valData));
		this->mConfMap.insert(make_pair(keyData, temp));
	}
}


Config Config::Parse(std::string&& filePath)
{
	try {
		fstream fs(filePath);
		if (!fs.is_open())
			throw runtime_error("Could not open file.");

		string temp;
		Config conf;//配置文件对象
		while (getline(fs, temp))//一行行读取配置文件信息
		{
			conf.ParseLineStr(temp);//解析单行数据
		}
		return conf;
	}
	catch (const exception& e)
	{
		throw runtime_error(string("libProp:An error occured when parsing the file,error message: ") + e.what());
	}
}

Config::Config(Config& conf)
{
	this->mConfMap = conf.mConfMap;
}

Config::Config()
{
}

Config::~Config()
{
}

Value* Config::operator[](const std::string& key)
{
	shared_lock<shared_mutex>lg(this->mLocker);
	return this->mConfMap[key];
}

void libProp::EraseFBSpace(std::string& data)
{
	data.erase(0, data.find_first_not_of(" "));//删除前面的空格
	data = data.substr(0, data.find_last_not_of(" ") + 1);//删除后面的空格
}
