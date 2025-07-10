// go_type_interface.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>

class Duck
{
public:
    std::string getName() const { return "a duck"; }
    void walk(int x, int y) { std::cout << "I can walk!" << std::endl; };
    void swim(int x, int y) { std::cout << "I can swim!" << std::endl; }
    void fly(int x, int y) { std::cout << "I can fly!" << std::endl; }
    void gaga(std::string gagawhat) { std::cout << "I can gaga! " << gagawhat <<  std::endl; }
};




template<typename T>
class TNamed
{
public:
	TNamed(T* pointer) : pointer(pointer) {}
	T* pointer;

	std::string getName() const
	{
		return pointer->getName();
	}
};

template<typename T>
class IBird
{
public:
	IBird(T* pointer) : pointer(pointer) {}
	T* pointer;

	void fly(int x, int y)
	{
		pointer->fly(x, y);
	}

	void gaga(std::string gagawhat)
	{
		pointer->gaga(gagawhat);
	}
};

template<typename T>
class IAnimal : public TNamed<T>, public IBird<T>
{
public:
	IAnimal(T* pointer) : TNamed<T>(pointer), IBird<T>(pointer) {}
};

int main()
{
    std::cout << "Hello World!\n";

	Duck duck;
	IAnimal<Duck> animal(&duck);
	animal.getName();
	animal.fly(1, 2);
	animal.gaga("hoho");

	int a = 0;
	int b = 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
