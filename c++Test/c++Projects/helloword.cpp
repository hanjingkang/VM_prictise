#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <cmath>
#include <algorithm>
#include <memory>
#include <unordered_map>

#include "class_school.h"
#include"class_smartptr.h"
#define HJK1
#define HJK2

#include "polymorphism.h"

using namespace std;

//文件读取
void fileread()
{
    string file = "D:\\c++Test\\c++Projects\\data.txt";
    ofstream out(file);
    out << 123 << endl;
    out << 3 << endl;
    out << 22123 << endl;

    out.close();
    ifstream read(file);
    string s;
    read >> s;
    cout << s << endl;
    read >> s;
    cout << s << endl;
    read >> s;
    cout << s << endl;
    read.close();
}

//lambda
void lambdaTest()
{
    int sz = 4;
    vector<string> word = {"abc", "apple", "banana"};
    vector<int> arr = {1, 2, 3, 4, 5, 6};
    //值捕获与引用捕获
    int val = 10;
    auto f1 = [=]() mutable
    { return ++val; }; //显式写作f1=[val]
    auto f2 = [&]() mutable
    { return ++val; }; //显式写作f1=[&val]
    val = 0;
    auto a = f1();
    auto b = f2();
    cout << "值捕获：" << a << "\n引用捕获:" << b << endl;

    for_each(word.begin(), word.end(), [](const string &s)
             { cout << s << "\n"; });

    auto lc = find_if(word.begin(), word.end(), [sz](const string &a)
                      { return a.size() >= sz; });
    sort(word.begin(), word.end(), [](const string &a, const string &b)
         { return a.size() > b.size(); });

    for (auto &s : word)
    {
        cout << s << " ";
    }
    cout << *lc;
    cout << endl;
}

//int long输出测试
void numberTest()
{
    unsigned int a = 0;
    long long b = 0;

    a = pow(2, 31);
    b = pow(2, 31);

    cout << a << "\n"
         << b << "\n";
}

//atoi
void myatoi()
{
    string s = "9+- ";
    for (auto &p : s)
    {
        cout << p << " ";
    }
}

//位运算实现四则运算

//加法
int add(int adder1, int adder2)
{
    int sum = 0, carry = 0;
    do
    {
        sum = adder1 ^ adder2;
        carry = (adder1 & adder2) << 1;
        adder1 = sum;
        adder2 = carry;
    } while (carry != 0);
    return sum;
}

//减法:一个数的负数表示为该数取反加1，则变成两个数相加的形式
int sub(int sub1, int sub2)
{
    return add(sub1, add(~sub2, 1));
}

//乘法:
int mul(int mul1, int mul2)
{
    int sum;
    while (mul1 != 0)
    {
        if (mul1 & 1 == 1)
        {
            sum = add(sum, mul2);
        }
        mul2 = mul2 << 1;
        mul1 = mul1 >> 1;
    }
    return sum;
}

//除法:未考虑溢出
int dive(int div, int div2)
{
    int fuhao = 0;
    if (div < 0)
    {
        div = -1 * div;
        fuhao++;
    }
    if (div2 < 0)
    {
        div2 = -1 * div2;
        fuhao++;
    }

    if (div < div2)
    {
        return 0;
    }
    int divC = 0, div2C = 0, i = 1;
    for (; (div >> i) > 0; i++)
        ;
    divC = i;
    i = 1;
    for (; (div2 >> i) > 0; i++)
        ;
    div2C = i, i = 0;
    int temp = 0, res = 0, ans = 1, flag = 0;
    if ((div2 << (divC - div2C)) > div)
    {
        temp = (div2 << (divC - div2C - 1));
        flag = 1;
    }
    else
    {
        temp = (div2 << (divC - div2C));
    }
    int count = 1;
    do
    {

        res = sub(div, temp);
        if (res < div2)
        {
            ans <<= flag ? (divC - div2C - count) : (divC - div2C - count + 1);
            break;
        }

        for (; temp > res; temp >>= 1, ans <<= 1, count++)
            ;
        ans |= 1;
        div = res;

    } while (1);

    return fuhao == 1 ? -1 * ans : ans;
}

//面试题必知必会

//语言基础篇
//1.malloc&free and new&delete
void nd_mf()
{
    //malloc&free:
    int *p = (int *)malloc(64); //整形指针指向64字节内存的地址
    //int *p=(int *)malloc(4*sizeof(int));//整形指针指向4个int整形空间的地址
    if (p == NULL)
    {
        printf("malloc内存分配错误\n");
    }
    printf("malloc内存分配成功\n");
    free(p);
    p = NULL; //及时置空

    //new&delete:
    //int *p=new int;//创建了整形对象，整型指针指向该对象首地址
    //int *p=new int();//同上，初始化0
    int *p2 = new int(1); //初始化1
    if (p2 == NULL)
    {
        printf("new内存分配错误\n");
    }
    printf("new内存分配成功\n");
    delete p2;
    p2 = NULL; //及时置空

    //new&delete创建数组：需要确定元素个数，加()初始化为0
    int *p4 = new int[10];
    int *p5 = new int[10]();
    delete[] p4;
    delete[] p5;
    p4 = NULL; //及时置空
    p5 = NULL; //及时置空

    //区别：         malloc&free        |        new&delete
    //属性：        stdlib.h库函数       |        编译器支持
    //参数：        指定大小             |        自动分配
    //返回值：      void,要强制转换      |      对应的指针,无需强转
    //分配失败：    返回NULL             |      抛出bac_alloc异常
    //自定义类型：  对于复杂的动态对象，new&delete可以调用其构造与析构函数，
    //              而malloc&free不行，后者只负责内存的分配与释放。
}

//2.a和&a有什么区别
void reference()
{
    int a[10] = {1, 3};
    int(*p)[10] = &a;                      //定义一个数组指针，初始化指向数组a
    printf("%d\n%d\n", *(a + 1), (*p)[1]); //数组指针的使用
    int *q = (int *)p;
    printf("%d\n", *q); //经过类型转换，整型指针指向数组首地址
}

//3.static
string class_school::lib="beijing library：";//static成员变量一般在类外初始化
void statictest()
{
    //修饰局部变量
    for (int i = 0; i < 5; i++)
    {
        static int mystatic; //只进行一次初始化
        mystatic++;
        printf("%d\n", mystatic);
    }

    //修饰成员变量
    class_school a,b,c,d;
    cout<<a.lib<<endl<<c.lib<<endl;
    class_school::lib="hangzhou library：";
    cout<<class_school::lib<<endl;

    //修饰成员函数：函数只能访问类的静态成员
    class_school::add(" 鲁迅全集");//可以不需要通过实例化对象直接访问
    a.add(" 毛选");//也可通过实例化对象访问
    cout<<b.lib<<endl<<class_school::lib<<endl;

    //其他：
    //修饰全局变量和函数时，在整个文件中可见，在文件外 不可见

    //C中的static和C++的static关键字有何区别:
    //c中：修饰 局部静态变量和外部 静态变量，函数
    //c++在此基础上还修饰静态成员和静态成员函数

}

//6.智能指针
void smartptr(){
    //自定义指针
    class_smartptr<int> ptr1(new int(1));
    class_smartptr<string> ptr2(new string("text string"));
    cout<<*ptr1<<endl;
    cout<<ptr2->c_str()<<endl;

    //智能指针unique_ptr
    unique_ptr<string> p1(new string("asd")),p2;//不能通过赋值初始化，可以为空
    p2=unique_ptr<string>(new string("asdas"));//允许临时右值的uniqueptr赋值
    p2=move(p1);//其他情况需要uniqueptr之间赋值时，使用move



    //智能指针share_ptr
    shared_ptr<int> ptr3(new int(2));
    shared_ptr<int> ptr4(new int(5));
    cout<<"p3 count:"<<ptr3.use_count()<<"  p3 value:"<<ptr3.get()<<endl;
    cout<<"p4 count:"<<ptr4.use_count()<<"  p4 value:"<<ptr4.get()<<endl;
    ptr4=ptr3;
    cout<<"p3 count:"<<ptr3.use_count()<<"  p3 value:"<<ptr3.get()<<endl;
    cout<<"p4 count:"<<ptr4.use_count()<<"  p4 value:"<<ptr4.get()<<endl;

    //其他：weak_ptr可以绑定到sharedptr，但不会增加其引用计数。
    //防止两个sharedptr相互引用时引用计数永不为0，产生死锁
}

//8.右值引用
void rvalue(){
    int &&r_ref=10;//定义一个右值引用，且必须要初始化,不能使用左值初始化
    cout<<r_ref;
    int lv=20;
    r_ref=100;//被右值修改
    cout<<r_ref;
    r_ref=lv;//被左值修改
    cout<<r_ref;

    //右值引用的好处：
    //当函数返回值是一个临时对象时，出了作用域就会被析构，若此时再用左值引用进行地址访问出错
    //当返回值为 右值时，将临时变量的内存占为己有，保持其有效性。
    
    //应用
    //移动语义:赋值，只发生资源转移，避免了拷贝的风险（悬空指针、内存开销）
    //完美转发:
}

//8.1不完美转发
void notperfectForward(int &&i){
    cout<<i;//参数i本来是右值,但是转发到函数内部后变为了左值
}
//8.2完美转发
template<class T>
void perfectForward(T &&i){
    cout<<forward<T>(i);//基于std::forward实现完美转发，左右值属性不变
}

//9.悬挂指针与野指针
void twopointer(){
    int *hangOn_ptr=new int(2);
    int *wild_ptr;//未初始化叫野指针

    cout<<"悬挂指针  释放前地址："<<hangOn_ptr<<" 值："<<*hangOn_ptr<<endl;
    delete hangOn_ptr;//释放，只是内存回收，指针不变，叫悬挂指针
    cout<<"悬挂指针  释放后地址："<<hangOn_ptr<<" 值："<<*hangOn_ptr<<endl;

    //cout<<"野指针  为初始化的地址："<<wild_ptr<<" 值："<<*wild_ptr<<endl;//报错
    wild_ptr=nullptr;
    cout<<"野指针  置空后的地址："<<wild_ptr<<endl;
    //如何规避： 野指针：及时初始化或置空，使用前检查有效性  悬挂指针：空间释放后及时置空

}

//11.变量的声明与定义
void externtest(){
    extern string outside_value;//来自文件外
    extern string outside_value2;//来自文件外
    outside_value2="init outsidevalue";
    extern string down_value;//来自函数之后
    cout<<outside_value.c_str()<<endl;
    cout<<outside_value2.c_str()<<endl;
    cout<<down_value.c_str()<<endl;
}
string down_value="i am a string below this fuc";

//12.条件编译
void ifdeftest(){
    #ifdef HJK1
    cout<<"def HJK1\n";
    #else
    cout<<"notdef HJK1\n";
    #endif // DEBUG

    #ifndef HJK2
    cout<<"notdef HJK2\n";
    #else
    cout<<"def HJK2\n";
    
    #endif // !HJHK2
}

//18.volatile
void volatiletest(){
    //volatile修饰变量说明其值后续易变，每次访问应当从内存读取而非寄存器。告诉编译器不优化
    //应用场景：嵌入式硬件、中断服务子程序、多线程共享变量
    volatile int a=0;
    //volatile和const修饰变量说明，变量在程序内不允许改变 ，但是由于程序外原因发生改变是允许的（内存上的值改变）
    volatile const int b=12;
}

//20.字符串拷贝：strcpy,sprintf,memcpy
void compare(){
    char str1[40];
    char str2[40];

    int num[4]={1,22,3,4};
    strcpy(str1,"abc");
    strcpy(str2,"123");
    cout<<str1<<endl<<str2<<endl;



    int n=sprintf(str2,"ans:%s%d%s\n",str1,520,"ddd");//数据拼接并返回字符个数
    cout<<n<<endl<<str2<<endl;

    memcpy(str1+3,str2,10);//对内存的拷贝。效率最高
    cout<<str1;



}

//22.指针与引用
void PtrAndRef(){
    class_school  a;
    class_school* ptr=&a;//指针有自己的空间
    class_school &ref=a;//引用只是一个别名

    //看大小
    cout<<"obj:"<<a.lib<<"obj size:"<<sizeof(a)<<endl;
    cout<<"ptr:"<<ptr<<"ptr size:"<<sizeof(ptr)<<endl;
    cout<<"ref:"<<ref.lib<<"ref size:"<<sizeof(ref)<<endl;

    //对对象的操作，指针要先解引用，引用可以直接操作

}

//23.指针常量：int const*p=x; 指针p本身是常量
//常量指针：const int *p=x; 被指的对象x是常量
//常量指针常量：const int const *p=x; p和x都是常量，不可修改，即指向常量的常量指针
//非常量指针：int *p=x; x不能是常量
//常量引用：const int &a=x;  x可以是常量也可以是普通变量
//非常量引用：int &a=x;  x只能是非常量
//顶层const：指针本身是常量 如*const 
//底层const：被指对象是常量 如const int



//27.          类          结构体
//函数成员      有           无
//访问等级      有           无
//继承关系      有           无


//面向对象篇

//3.多态
void polymorphism(){

    //赋值兼容规则：在需要基类对象的任何地方可以用子类对象来代替。（多态的前提）
    parent *base=NULL;
    parent p1(20);
    child c1(30);//建立一个子类对象时，会沿继承路径依次调用基类的构造函数
    base=&p1;
    base->print();//打印父类
    base=&c1;
    base->print();//不加virtual打印父类,加virtual打印子类，支持多态。
    p1=c1;
    p1.print();//打印父类
    parent &base2=c1;
    base2.print();//不加virtual打印父类,加virtual打印子类，支持多态。
    parent *base3=&c1;
    base3->print();//不加virtual打印父类,加virtual打印子类，支持多态。
    //child &child=p1;//报错
    //总结：
    //1.派生类对象可以初始化对基类对象的引用，但只能使用从基类继承的成员
    //2.1 静多态：比如函数重载，也算多态，通过命名在编译阶段决定其类型。
    //2.2 动多态：在运行阶段决定其类型，如成员函数加virtual成虚函数。
    //动多态条件：基类虚函数、子类override、基类指针/引用指向子类对象




}

//18.强制类型转换
void cast_test(){
    //1.static_cast
    int a=1,b=2;
    double c=static_cast<double>(a)/static_cast<double>(b);
    cout<<c<<endl;
    //应用场景：
    //父子类间指针引用的转换：子转父安全，父转子不安全
    //基本数据类型间转换
    //改变空指针的类型
    //其他转void

    //2.const_cast:去除 const\volatail属性
    //注意const_cast去除常量性对象必须为指针或引用，而不是常量变量。前者转换为非常量指针、非常量引用
    const int a1=10;
    const int *p=&a1;
    int *q;
    q=const_cast<int*>(p);
    *q=30;//允许
    cout<<*q<<endl;

    //3.reinterpret_cast
    //用改变于指针或引用的用途
    int *x=new int(123);
    double *y=reinterpret_cast<double*>(x);
    cout<<*y<<endl;

    //4.dynamic_cast
    //用于动态类型的转换，只能用于有虚函数的类，只能转换指针和引用。有向上向下两种转换（父子类之间转换）
    


}



//STL篇

//2、map与hashmap
void map_hashmap(){
    //map基于红黑树，元素是有序排列的。有序性查找快，空间大，适用有序查找
    //unordered_map是c++11对hashmap的实现，基于hash表。查找快（快于map），建表慢，适用海量数据查找。
    //um处理哈希冲突的方法：开链法（拉链法）.
    //hashtable实现：内置28个质数，选择质数x建表（x>=元素个数).hashtable容量=bucket容量=x，bucket超过x则重新选x建表


    //map的几种插入方式
    unordered_map<int,int>mp;
    mp.insert(pair<int,int>(1,2));
    mp.insert(unordered_map<int,int>::value_type(2,3));
    mp.insert(make_pair(3,4));
    mp[4]=5;
    cout<<mp[1]<<mp[2]<<mp[3]<<mp[4]<<endl;

}

//4.vector相关
void  aboutvector(){
    //1.底层原理
    vector<int>array={1,2,3}; //是一种动态数组，元素超过endofstorage，那么重新申请空间（1.5、2倍）。原有迭代器失效
    vector<int>array2(2,4);
    cout<<"元素数量："<<array.size()<<"容量："<<array.capacity()<<endl;
    //2.reserve与resize区别
    array.reserve(5);
    array.push_back(122);
    cout<<"(after reserve)元素数量："<<array.size()<<"容量："<<array.capacity()<<"array[3]:"<<array[3]<<endl;//此时容器达到5个元素时扩容。
    array.resize(10,33);
    cout<<"(after resize)元素数量："<<array.size()<<"容量："<<array.capacity()<<"array[3]:"<<array[3]<<endl;//改变空间大小，并可以对初始化多出来空间的一部分元素
    array.push_back(333);
    cout<<"(after resize and push_back)元素数量："<<array.size()<<"容量："<<array.capacity()<<"array[4]:"<<array[10]<<endl;//改变空间大小，并可以对初始化多出来空间的一部分元素
    //3.size与capacity区别
    //size=finish-start;capacity=endofstorage-start;endofstorge是扩容的临界点

    //4.vector的元素类型不能是引用，vector底层是对对象的连续排列，引用不是对象

    //5.迭代器失效：操作引发内存重新分配比如：insert；删除元素操作(用erase来删除可以保留迭代器的有效性（返回下一个元素的迭代器）)
    auto it1=array.begin();
    cout<<"迭代器1："<<&it1<<endl;
    array.insert(array.begin()+2,22);
    auto it2=array.begin();
    cout<<"迭代器2："<<&it2<<endl;
    auto it3=array.begin()+3;
    cout<<"迭代器3："<<&it3<<endl;
    auto it4=array.erase(it3);//返回下一个元素的迭代器
    cout<<"迭代器4："<<&it4<<endl;

    //6.关于释放内存
    //clear():清内容不清内存
    //swap():清内容内存,交换数据，得到新vector
    cout<<"array元素数量："<<array.size()<<"array容量："<<array.capacity()<<endl;
    cout<<"array2元素数量："<<array2.size()<<"array2容量："<<array2.capacity()<<endl;
    array.swap(array2);
    cout<<"array元素数量："<<array.size()<<"array容量："<<array.capacity()<<endl;
    cout<<"array2元素数量："<<array2.size()<<"array2容量："<<array2.capacity()<<endl;

    //shrink_to_fit:降低capacity与size匹配//清内容清内存
    array2.shrink_to_fit();
    cout<<"(after shrink)array2元素数量："<<array2.size()<<"array2容量："<<array2.capacity()<<endl;

    //完全清内存
    vector<int>().swap(array);//与一个临时空对象交换内存，临时对象失效 的同时，本身达到内存清理的目的
    cout<<"(after Vec_swap)array元素数量："<<array.size()<<"array容量："<<array.capacity()<<endl;








    
}


//5.list相关
void aboutlist(){
     
}


//16.容器内删除一个元素 
void howtoerase(){
    //如果是顺序容器
    vector<int> a={1,2,3,4};
    for(auto it=a.begin();it!=a.end();it++){
        if (*it==3)
        {
            it=a.erase(it);//通过erase返回迭代器，保持迭代器 有效性
        }
        
    }

    //如果是关联容器
    unordered_map<int,int> mp={{1,2},{2,3},{3,4}};
    for(auto it=mp.begin();it!=mp.end();){
        if (it->first==2)
        {
            mp.erase(it++);//erase不会返回迭代器，直接自增。 
        }
        
    }

}


//内存管理篇

//2.delete 与 delete []
void aboutdel(){

    int *a=new int(12);
    int *array=new int[12];

    cout<<a<<endl;
    cout<<(array+1)<<endl;
    delete   a;
    delete [] array;
    cout<<a<<endl;
    cout<<(array+1)<<endl;
    //对于简单类型，不管new()或new[]都可以用delete/delete[]进行内存释放。
    //对于自定义对象，单个对象 用new（）/delete。对象数组用new[]/delete[]
    //使用delete/free后，内存不回收，指针值不变，为避免野指针，及时置空。

}

//4.内存泄露的场景
void memoryloss(){
    //1.malloc/free没有成对出现，new/delete没有成对出现

    //2.拷贝类的时候，可能会出现两个指针指向同一块内存，那么执行各自析构函数，对同一块内存两次释放，是不允许的。因此要针对性的定义拷贝/赋值构造函数

    //3.没有将基类析构函数定义为虚函数

    //4.linux内存泄露的检测方式：valgrind、mtrace
}





int main()
{
    SetConsoleOutputCP(CP_UTF8);
    aboutdel();
    return 0;
}