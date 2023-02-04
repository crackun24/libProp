# Proplib:一个简单的properties解析库
### 使用方法简述
- 导入头文件 "libProp"
- 使用Config对象的静态构造函数Parse()构造配置文件对象
- 用Config类的"[]"重载函数访问数值,并使用 "as<类型>()" 函数写出类
- 若为数组使用"[元素位置]"访问数组中的元素


### 使用示例
##### 假设有一个配置文件如下

#### 文件名: 
~~~
xxx.properties
~~~
#### 文件内容
~~~
testKey = testVal
testArray = [ val1 , 1234 ]
~~~
##### 解析该文件的代码
~~~ 
#include <libProp>//导入头文件

int main(){
	libProp::Config conf = libProp::Config::Parse("./xxx.properties");
	cout << conf["testKey"].as<string>();//输出 testVal
	cout << conf["testArray"][0].as<string>();//输出 val1
	cout << conf["testArray"][1].as<int>();//输出 1234
}
~~~
