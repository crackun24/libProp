# Proplib:һ���򵥵�properties������
### ʹ�÷�������
- ����ͷ�ļ� "libProp"
- ʹ��Config����ľ�̬���캯��Parse()���������ļ�����
- ��Config���"[]"���غ���������ֵ,��ʹ�� "as<����>()" ����д����
- ��Ϊ����ʹ��"[Ԫ��λ��]"���������е�Ԫ��


### ʹ��ʾ��
##### ������һ�������ļ�����

#### �ļ���: 
~~~
xxx.properties
~~~
#### �ļ�����
~~~
testKey = testVal
testArray = [ val1 , 1234 ]
~~~
##### �������ļ��Ĵ���
~~~ 
#include <libProp>//����ͷ�ļ�

int main(){
	libProp::Config conf = libProp::Config::Parse("./xxx.properties");
	cout << conf["testKey"].as<string>();//��� testVal
	cout << conf["testArray"][0].as<string>();//��� val1
	cout << conf["testArray"][1].as<int>();//��� 1234
}
~~~
