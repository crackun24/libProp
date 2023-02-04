#ifndef LIBPROP_
#define LIBPROP_

#ifndef EXEC_TYPE
#define EXEC_TYPE _declspec(dllexport)
#else
#define EXEC_TYPE _declspec(dllimport)
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <shared_mutex>
#include <map>

namespace libProp {
	void EraseFBSpace(std::string &data);//ȥ���ַ���ǰ��Ŀո�
	enum ValType {
		ValueType//��������
		, ArrayType//��������
	};//�������͵�ö��


	class Value {//������ 
	private:
		ValType mType;//��������
		union
		{
			std::string* mVal;//ָ�����ݵ�ָ��
			std::vector<std::string>* mArray;//ָ�������ָ��
		};
	public:
		Value(std::string* data);//��ֵ���͵Ĺ��캯��
		Value(std::vector<std::string>* array);//�������͵Ĺ��캯��
		Value(Value& val);//�������캯��
		template<typename T>
		T as();//�����ض����͵�ֵ
		~Value();//��������,�ͷŶ�������Դ
	};

	class Config//���ö���
	{
	private:
		std::map<std::string, Value*>mConfMap;//�����ļ�ӳ���
		std::shared_mutex mLocker;//������
		void ParseLineStr(std::string& data);//����һ������
	public:
		static Config Parse(std::string&& filePath);//���������ļ�
		Config(Config& conf);//�������캯��
		Config();//���캯��
		~Config();
		Value* operator[](const std::string& key);//��key��ȡvalָ��
	};

	template<typename T>
	inline T Value::as()
	{
		return static_cast<T>(*this->mVal);
	}
}

#endif // LIBPROP_
