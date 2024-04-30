#include "libProp.h"

using namespace std;
using namespace libProp;

void libProp::Value::CheckType(ValType type)
{
	if (this->type != type)
		throw runtime_error("type error.");
}

Value::Value(string data) :Value()
{
	this->type = ValueType;
	this->mData = data;
}

Value::Value(vector<Value> array) :Value()
{
	this->type = ArrayType;//设置为数组的类型
	this->mArray = move(array);
}

Value::Value() :isDataChanged(false)
{
}


libProp::Value::operator int()
{
	try {
		return stoi(this->mData);
	}
	catch (const exception& e)
	{
		throw runtime_error(errMsgPrefix + e.what());
	}
}

Value::operator double()
{
	try {
		return stod(this->mData);
	}
	catch (const exception& e)
	{
		throw runtime_error(errMsgPrefix + e.what());
	}
}

Value::operator float() {
	try {
		return stof(this->mData);
	}
	catch (const exception& e)
	{
		throw runtime_error(errMsgPrefix + e.what());
	}
}

Value::operator long long()
{
	try {
		return stoll(this->mData);
	}
	catch (const exception& e) {
		throw runtime_error(errMsgPrefix + e.what());
	}
}

Value::operator std::string()
{
	try {
		return this->mData;
	}
	catch (const exception& e)
	{
		throw runtime_error(errMsgPrefix + e.what());
	}
}

Value libProp::Value::operator[](int index)
{
	try {
		CheckType(ArrayType);//判断是否为数组的类型
		Value temp = this->mArray.at(index);//获取数组下标的内容
		return temp;//返回获取的内容
	}
	catch (const exception& e)
	{
		throw runtime_error(errMsgPrefix + e.what());
	}
}

int libProp::Value::size()
{
	try {
		CheckType(ArrayType);//判断是否为数组的类型
		return this->mArray.size();//返回数组的大小
	}
	catch (const exception& e)
	{
		throw runtime_error(errMsgPrefix + e.what());
	}
}

Value::operator long()
{
	try {
		return stol(this->mData);
	}
	catch (const exception& e)
	{
		throw runtime_error(errMsgPrefix + e.what());
	}
}

Value::operator bool()
{
	try
	{
		if (this->mData == "true")
		{
			return true;
		}
		else if (this->mData == "false")
		{
			return false;
		}
		else//数据不合法
		{
			throw runtime_error("invalid bool data.");
		}

	}
	catch (const exception& e)
	{
		throw runtime_error(errMsgPrefix + e.what());
	}
}

Value::~Value()
{
}

std::string Value::toFileStr()
{
	if (this->type == ValueType)
	{
		return this->mData;
	}
	else if (this->type == ArrayType)
	{
		stringstream tempData;
		tempData << "[";
		for (auto it = this->mArray.begin(); it != this->mArray.end(); it++)
		{
			if (it != this->mArray.end())
			{
				tempData << (*it).toFileStr() << ",";
			}
		}
		tempData << "]";
	}
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

		vector<Value> temp;
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
			temp.push_back(tempVal);//插入元素
		}
		Value tempVal(move(temp));
		this->mConfMap.insert(make_pair(keyData, move(tempVal)));
	}
	else {
		Value tempVal(move(valData));
		this->mConfMap.insert(make_pair(keyData, move(tempVal)));
	}

}


bool Config::isDataChanged()
{
	bool isChanged = false;//数据是否改变

	for (auto& it : this->mConfMap)//遍历数据,判断数据是否有更改
	{
		isChanged ^= it.second.isDataChanged;
	}

	return isChanged;
}

Config Config::Parse(std::string&& filePath)
{
	try {
		fstream fs(filePath);
		if (!fs.is_open())
		{
			throw runtime_error("Could not open file.");
		}

		string temp;
		Config conf;//配置文件对象

		while (getline(fs, temp))//一行行读取配置文件信息
		{
			conf.ParseLineStr(temp);//解析单行数据
		}

		fs.close();//关闭文件
		conf.mFilePath = filePath;

		return conf;
	}
	catch (const exception& e)
	{
		throw runtime_error(string("libProp:An error occured when parsing the file,error message: ") + e.what());
	}
}



Config libProp::Config::operator=(const Config& conf)
{
	this->mConfMap = conf.mConfMap;
	this->mFilePath = conf.mFilePath;
	return *this;
}

void Config::save()
{
	for (auto& it : this->mConfMap)
	{
		it.second.isDataChanged = false;//更新保存状态
	}

	stringstream data;//写入到硬盘的数据
	for (auto it : this->mConfMap)//遍历哈希表
	{
		data << it.first << " = " << it.second.toFileStr() << endl;
		cout << "key:" << it.first << endl;
	}

	cout << "file data:" << data.str();

	fstream fs(this->mFilePath, ios::out);
	if (!fs.is_open())
	{
		throw runtime_error("Could not open the config file for writing.");
	}

	fs << data.str();
	fs.close();//关闭文件
	cout << "saved" << endl;//BUG

}


Config::Config()
{
}

Config::~Config()
{
	if (isDataChanged())//判断是否需要保存
	{
		this->save();
	}
}

Value& Config::operator[](const std::string& key)
{
	if (this->mConfMap.find(key) == this->mConfMap.end())//无法找到键值
	{
		throw runtime_error("Could not find the key.");
	}

	return this->mConfMap[key];
}

ConfigGuard::ConfigGuard(Config& conf) :mConfig(conf)
{}

Value ConfigGuard::operator[](const std::string& key)
{
	lock_guard<shared_mutex>lg(this->mLocker);
	return this->mConfig[key];
}

Value ConfigGuard::GetEntry(const std::string& key)
{
	lock_guard<shared_mutex>lg(this->mLocker);
	return this->mConfig[key];
}

void libProp::EraseFBSpace(std::string& data)
{
	data.erase(0, data.find_first_not_of(" "));//删除前面的空格
	data = data.substr(0, data.find_last_not_of(" ") + 1);//删除后面的空格
}
