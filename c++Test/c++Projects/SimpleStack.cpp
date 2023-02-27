#include <iostream>
#include <string>
#include <exception>

using namespace std;

class E : public std::exception
{
    const char *msg;
    E(){};

public:
    explicit E(const char *s) throw() : msg(s){};
    const char *what() const throw() { return msg; }
};
template <typename T>
class bwstack
{
private:
    /* data */
    static const int defaultsize = 10;
    static const int maxsize = 100;
    int _size;
    int _top;
    T *_stkptr;

public:
    explicit bwstack(/* args */ int s = defaultsize);
    ~bwstack() { delete[] _stkptr; }
    T &push(const T &);
    T &pop();
    bool isempty() const { return _top < 0; }
    bool isfull() const { return _top >= _size - 1; }
    int size() const { return _size; }
    int top() const { return _top; }
};

template <typename T>
bwstack<T>::bwstack(int s)
{
    if (s > maxsize || s < 1)
        throw E("invalid stack size");
    else
        _size = s;
    _stkptr = new T[_size];
    _top = -1;
}

template <typename T>
T &bwstack<T>::push(const T &i)
{
    if (isfull())
        throw E("stack full");
    return _stkptr[++_top] = i;
}

template <typename T>
T &bwstack<T>::pop()
{
    if (isempty())
        throw E("stack empty");
    return _stkptr[_top--];
}

int main(int argc, char **argv)
{
    try
    {
        bwstack<int> si(5);
        cout << "si size:" << si.size() << endl;
        cout << "si top:" << si.top() << endl;
        for (int i : {1, 2, 3, 4, 5})
        {
            si.push(i);
        }
        cout << "si top after pushing" << si.top() << endl;
        cout << "si is:" << (si.isempty() ? "" : "not") << "full" << endl;

        while (!si.isempty())
        {
            cout << "pooped:" << si.pop() << endl;
            /* code */
        }
    }
    catch (E &e)
    {
        std::cerr << "stack error:" << e.what() << endl;
    }

    system("pause");
    return 0;
}