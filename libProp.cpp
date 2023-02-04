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
	this->mType = ArrayType;//����Ϊ���������
	this->mArray = array;
}

Value::~Value()
{
	if (this->mType == ValType::ValueType)//Ϊ��������
		delete this->mVal;//�ͷ�������Դ
	else if (this->mType == ValType::ArrayType)
		delete this->mArray;
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

		vector<string>* temp = new vector<string>;
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
			EraseFBSpace(tempVal);//ɾ��ǰ��Ŀո�
			temp->push_back(tempVal);
		}

		Value* val = new Value(temp);
		this->mConfMap.insert(make_pair(keyData, val));//�����ֵӳ���ϵ
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
		Config conf;//�����ļ�����
		while (getline(fs, temp))//һ���ж�ȡ�����ļ���Ϣ
		{
			conf.ParseLineStr(temp);//������������
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
	data.erase(0, data.find_first_not_of(" "));//ɾ��ǰ��Ŀո�
	data = data.substr(0, data.find_last_not_of(" ") + 1);//ɾ������Ŀո�
}
