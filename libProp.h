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
	void EraseFBSpace(std::string &data);//去除字符串前后的空格
	enum ValType {
		ValueType//数据类型
		, ArrayType//数组类型
	};//数据类型的枚举


	class Value {//数据类 
	private:
		ValType mType;//数据类型
		union
		{
			std::string* mVal;//指向数据的指针
			std::vector<std::string>* mArray;//指向数组的指针
		};
	public:
		Value(std::string* data);//数值类型的构造函数
		Value(std::vector<std::string>* array);//数组类型的构造函数
		Value(Value& val);//拷贝构造函数
		template<typename T>
		T as();//返回特定类型的值
		~Value();//析构函数,释放堆区的资源
	};

	class Config//配置对象
	{
	private:
		std::map<std::string, Value*>mConfMap;//配置文件映射表
		std::shared_mutex mLocker;//互斥锁
		void ParseLineStr(std::string& data);//解析一行数据
	public:
		static Config Parse(std::string&& filePath);//解析配置文件
		Config(Config& conf);//拷贝构造函数
		Config();//构造函数
		~Config();
		Value* operator[](const std::string& key);//用key获取val指针
	};

	template<typename T>
	inline T Value::as()
	{
		return static_cast<T>(*this->mVal);
	}
}

#endif // LIBPROP_
