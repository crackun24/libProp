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
#include <unordered_map>

namespace libProp {
	enum ValType {
		ValueType//��������
		, ArrayType//��������
	};//�������͵�ö��


	class EXEC_TYPE Value {//������ 
	private:
		ValType mType;//��������
		union
		{
			std::string* mVal;//ָ�����ݵ�ָ��
			std::vector<std::string>* mArray;//ָ�������ָ��
		};

	public:

		~Value();//��������,�ͷŶ�������Դ
	};

	class EXEC_TYPE Config//���ö���
	{
	private:
		std::unordered_map<std::string, Value*>mConfMap;//�����ļ�ӳ���
		std::shared_mutex mLocker;//������
		void ParseLineStr(std::string &data);//����һ������
	public:
		static Config Parse(std::string &filePath);//���������ļ�
		Value* operator[](const std::string& key);//��key��ȡvalָ��
	};
}

#endif // LIBPROP_
