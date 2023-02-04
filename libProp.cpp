#include "libProp.h"

using namespace std;
using namespace libProp;

Value::~Value()
{
	if (this->mType == ValType::ValueType)//Ϊ��������
		delete this->mVal;//�ͷ�������Դ
	else if (this->mType == ValType::ArrayType)
		delete this->mArray;
}

void libProp::Config::ParseLineStr(std::string& data)
{
	if (data.find("=") == string::npos)//�ж��Ƿ�Ϊ��Ч����
		return;//������Ч�з���
}


Config Config::Parse(std::string& filePath)
{
	try {
		fstream fs(filePath);
		if (fs.is_open())
			throw runtime_error("Could not open file.");

		string temp;
		Config conf;//�����ļ�����
		while (getline(fs, temp))//һ���ж�ȡ�����ļ���Ϣ
		{
			conf.ParseLineStr(temp);//������������
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
