#include <iostream>
using namespace std;
template <class T>
class class_smartptr
{
private:
    T *ptr;

public:
    class_smartptr(T *_ptr) : ptr(_ptr){}
    ~class_smartptr()
    {
        if (ptr != nullptr)
        {
            cout << "smartptr delete\n";
            delete ptr;
            ptr = nullptr;
        }
    }

    T &operator*()
    {
        return *ptr;
    }

    T *operator->()
    {
        return ptr;
    }
};
