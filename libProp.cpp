#include "libProp.h"

using namespace std;
using namespace libProp;

void libProp::Value::CheckType(ValType type)
{
	if (this->mType != type)
		throw runtime_error("type error.");
}

Value::Value(string data)
{
	this->mType = ValueType;
	this->mData = data;
}

Value::Value(vector<Value> array)
{
	this->mType = ArrayType;//����Ϊ���������
	this->mArray = move(array);
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
	catch (const exception& e){
		throw runtime_error(errMsgPrefix + e.what());
	}
}

Value libProp::Value::operator[](int index)
{
	try {
		CheckType(ArrayType);//�ж��Ƿ�Ϊ���������
		Value temp = this->mArray.at(index);//��ȡ�����±������
		return temp;//���ػ�ȡ������
	}
	catch (const exception& e)
	{
		throw runtime_error(errMsgPrefix + e.what());
	}
}

int libProp::Value::size()
{
	try {
		CheckType(ArrayType);//�ж��Ƿ�Ϊ���������
		return this->mArray.size();//��������Ĵ�С
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

Value::operator string()
{
	try {
		return this->mData;
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
		if(this->mData == "true")
		{
			return true;
		}else if(this->mData == "false")
		{
			return false;
		}else//���ݲ��Ϸ�
		{
			throw runtime_error("invalid bool data.");
		}
		
	}catch (const exception &e)
	{
		throw runtime_error(errMsgPrefix + e.what());
	}
}

Value::~Value()
{
}

void Config::ParseLineStr(std::string& data)
{
	if (data == "" || data.find("=") == string::npos)//�ж��Ƿ�Ϊ��Ч����
		return;//������Ч�з���
	string& keyData = data.substr(0, data.find("="));//��ȡ�Ⱥ�ǰ�������
	string& valData = data.substr(data.find("=") + 1);//��ȡ�Ⱥź��������
	EraseFBSpace(keyData);//ȥ��ǰ��Ŀո�
	EraseFBSpace(valData);//ȥ������Ŀո�

	if (*valData.begin() == '[')//�ж��Ƿ�������ĸ�ʽ��ͷ
	{
		valData.erase(0, 1);
		valData.pop_back();//ɾ��ĩβԪ��

		vector<Value> temp;
		while (valData != "")//û����ȡ�����һֱѭ����ȡ
		{
			auto pos = valData.find(",");//��ȡ�ָ�����λ��
			string tempVal;
			if (pos == string::npos)//�ж��Ƿ��зָ���
			{
				tempVal = valData;
				valData.clear();//û�зָ�����Ϊ���һ��Ԫ��
			}
			else {
				tempVal = valData.substr(0, pos);
				valData.erase(0, pos + 1);//ɾ�������ָ����ĵ��ѽ���Ԫ��
			}
			temp.push_back(tempVal);//����Ԫ��
		}
		Value tempVal(move(temp));
		this->mConfMap.insert(make_pair(keyData, move(tempVal)));
	}
	else {
		Value tempVal(move(valData));
		this->mConfMap.insert(make_pair(keyData, move(tempVal)));
	}

}


Config Config::Parse(std::string&& filePath)
{
	try {
		fstream fs(filePath);
		if (!fs.is_open())
			throw runtime_error("Could not open file.");

		string temp;
		Config conf;//�����ļ�����
		while (getline(fs, temp))//һ���ж�ȡ�����ļ���Ϣ
		{
			conf.ParseLineStr(temp);//������������
		}
		return move(conf);
	}
	catch (const exception& e)
	{
		throw runtime_error(string("libProp:An error occured when parsing the file,error message: ") + e.what());
	}
}

Config::Config(const Config& conf)
{
	this->mConfMap = conf.mConfMap;
}


Config libProp::Config::operator=(const Config& conf)
{
	this->mConfMap = conf.mConfMap;
	return *this;
}

Config::Config()
{
}

Config::~Config()
{
}

Value Config::operator[](const std::string& key)
{
	return this->mConfMap[key];
}

void libProp::EraseFBSpace(std::string& data)
{
	data.erase(0, data.find_first_not_of(" "));//ɾ��ǰ��Ŀո�
	data = data.substr(0, data.find_last_not_of(" ") + 1);//ɾ������Ŀո�
}

