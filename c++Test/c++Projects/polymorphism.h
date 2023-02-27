#include <iostream>

using namespace std;

//多态实践

//父类
class parent
{
private:
    int a;
public:
    parent(int a){
        this->a=a;
        cout<<"parent a:"<<a<<endl;
    }
    //虚函数定义：
    virtual void  print(){
        cout<<"parent 打印a: "<<a<<endl;
    }

    //纯虚函数的定义：virtual void draw()=0; 
    //含有纯虚函数的类叫抽象基类，不可实例化，存在的意义就是被继承，或者说，提供接口
    //若子类中没有对纯虚函数定义，则子类仍为抽象基类
    ~parent(){
        
    }
    //成员函数、析构函数可以为虚函数；静态成员函数、内联函数、构造函数不能为虚函数
};
//子类
class child: public parent
{
private:
    int b;
public:
    child(int b): parent(10){
        this->b=b;
        cout<<"child b:"<<b<<endl;
    }
    ~child(){

    }
    void draw(){
        cout<<"child调用纯虚函数"<<endl;
    }

    //继承自父类的虚函数，因此也是虚函数，virtual可加可不加
    void print(){
        cout<<"child  打印b："<<b<<endl;
    }
};




