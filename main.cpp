#include <iostream>
#include <cstdio>
#include <vector>
#include <list>
#include <memory>

using namespace std;

typedef shared_ptr<int> SharedPtr;
typedef unique_ptr<int> UniquePtr;
typedef weak_ptr<int> WeakPtr;


class HasPtr {
public:

    // HasPtr();
    HasPtr(const string &s);
    HasPtr(const HasPtr& other);
    HasPtr& operator=(const HasPtr& other);
    ~HasPtr();
    void print();
private:
    int* count;
    string* ps;
};

// HasPtr::HasPtr()
// {
//     cout << "默认构造函数" << endl;
// }
HasPtr::HasPtr(const string &s = string()): ps(new string(s)), count(new int(1))
{
    // cout << "构造函数" << endl;
}

HasPtr::HasPtr(const HasPtr& other): ps(other.ps), count(other.count)
{
    *count += 1;
    // cout << "拷贝构造函数" << endl;
}

HasPtr& HasPtr::operator=(const HasPtr& other)
{
    cout << "赋值运算符" << endl;
    if (--*count == 0)
    {
        delete ps;
        delete count;
        count == nullptr;
    }
    count = other.count;
    *count += 1;
    ps = other.ps;
    return *this;
}


HasPtr::~HasPtr()
{
    if (--*count == 1)
    {
        cout<<"delete count"<<endl;
        delete count;
        delete ps;
        count = NULL;
    }
    // cout << "析构函数" << endl;
}

void HasPtr::print() {
    cout << "count = " << *count << endl;
    // cout << "count= " << ps.use_count() << endl;
    cout << "ps = " << *ps << endl;
}
int main(int argc, char const *argv[])
{
    HasPtr obj("hello");
    HasPtr obj2("world");
    HasPtr obj3 = obj2;
    obj2 = obj;
    // HasPtr obj3 = obj;
    obj.print();
    obj2.print();
    obj3.print();
    // obj = obj2;
    // obj.print();
    // obj2.print();
    // obj3.print();
    // obj2 = obj;
    // obj.print();
    // obj2.print();
    // HasPtr obj2 = obj;
    // HasPtr obj2;
    // obj2 = obj;
    // obj.print();
    // obj2.print();
}
