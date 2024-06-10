#ifndef LIBPROP_
#define LIBPROP_

#ifndef EXEC_TYPE
#define EXEC_TYPE _declspec(dllexport)
#else
#define EXEC_TYPE _declspec(dllimport)
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <shared_mutex>
#include <sstream>
#include <map>

namespace libProp {
	void EXEC_TYPE EraseFBSpace(std::string& data);//去除字符串前后的空格
	enum ValType {
		ValueType//数据类型
		, ArrayType//数组类型
	};//数据类型的枚举


	class EXEC_TYPE Value {//数据类 
	private:
		const std::string errMsgPrefix = "libProp:An error occured,error message: ";//错误信息的前缀
		std::string mData;//数据
		std::vector<Value>mArray;//数组
		void CheckType(ValType type);//检查类型是否符合传入的参数,若不符合则抛出不符合的异常

	public:
		ValType type;//数据类型 
		bool isDataChanged;//数据是否被更改,如果更改则写入到硬盘中
		Value(std::string data);//数值类型的构造函数
		Value(std::vector<Value> array);//数组类型的构造函数
		Value();//设置为默认的构造函数
		operator int();//转换为int的函数
		operator long();//转换为Long的函数
		operator double();//转换为double的函数
		operator float();//转换为float的函数
		operator long long();//转换为long long 的函数
		operator std::string();//转换为字符串
		operator bool();//转换为bool的函数
		Value operator [](int index);//获取该位置的数组的内容
		int size();//获取数组的大小

		template<typename T>
		T as() {
			return static_cast<T>(this->mData);
		}//返回特定类型的值
		~Value();//析构函数,释放堆区的资源

		template<typename  T>
		Value& operator=(const T& data)
		{
			std::stringstream ss;
			ss << std::boolalpha;
			ss << data;
			this->mData = ss.str();
			this->isDataChanged = true;
			return *this;
		}

		std::string toFileStr();//转换为配置文件中的字符串
	};

	class EXEC_TYPE Config//配置对象
	{
	private:
		std::map<std::string, Value>mConfMap;//配置文件映射表
		std::vector<std::string>mCommentsList;//注释列表
		void ParseLineStr(std::string& data);//解析一行数据
		std::string mFilePath;//配置文件的路径
		bool isDataChanged();//判断数据是否改变

	public:
		static Config Parse(std::string&& filePath);//解析配置文件
		Config(const Config& conf) = default;//拷贝构造函数
		Config operator=(const Config& conf);//等于号重载函数
		void save();//保存到硬盘中
		Config();//构造函数
		~Config();
		Value& operator[](const std::string& key);//用key获取val指针
	};

}

#endif // LIBPROP_
