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
	void EXEC_TYPE EraseFBSpace(std::string& data);//ȥ���ַ���ǰ��Ŀո�
	enum ValType {
		ValueType//��������
		, ArrayType//��������
	};//�������͵�ö��


	class EXEC_TYPE Value {//������ 
	private:
		const std::string errMsgPrefix = "libProp:An error occured,error message: ";//������Ϣ��ǰ׺
		ValType mType;//�������� 
		std::string mData;//����
		std::vector<Value>mArray;//����
		void CheckType(ValType type);//��������Ƿ���ϴ���Ĳ���,�����������׳������ϵ��쳣
	public:
		Value(std::string data);//��ֵ���͵Ĺ��캯��
		Value(std::vector<Value> array);//�������͵Ĺ��캯��
		Value() = default;//����ΪĬ�ϵĹ��캯��
		operator int();//ת��Ϊint�ĺ���
		operator long();//ת��ΪLong�ĺ���
		operator double();//ת��Ϊdouble�ĺ���
		operator float();//ת��Ϊfloat�ĺ���
		operator long long();//ת��Ϊlong long �ĺ���
		operator std::string();//ת��Ϊ�ַ���
		Value operator [](int index);//��ȡ��λ�õ����������
		int size();//��ȡ����Ĵ�С

		template<typename T>
		T as() {
			return static_cast<T>(*this);
		}//�����ض����͵�ֵ
		~Value();//��������,�ͷŶ�������Դ
	};

	class EXEC_TYPE Config//���ö���
	{
	private:
		std::map<std::string, Value>mConfMap;//�����ļ�ӳ���
		void ParseLineStr(std::string& data);//����һ������
	public:
		static Config Parse(std::string&& filePath);//���������ļ�
		Config(Config& conf);//�������캯��
		Config(Config&& conf);//�ƶ����캯��
		Config operator=(Config& conf);//���ں����غ���
		Config();//���캯��
		~Config();
		Value operator[](const std::string& key);//��key��ȡvalָ��
	};
}

#endif // LIBPROP_
