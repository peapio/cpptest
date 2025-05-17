
#include "class_test.h"

// HasPtr::HasPtr()
// {
//     cout << "默认构造函数" << endl;
// }
HasPtr::HasPtr(const std::string &s = std::string()): ps(new std::string(s)), count(new int(1))
{
    // dbg("构造函数");
}

HasPtr::HasPtr(const HasPtr& other): ps(other.ps), count(other.count)
{
    *count += 1;
    // dbg("拷贝构造函数");
}

HasPtr& HasPtr::operator=(const HasPtr& other)
{
    dbg("赋值运算符");
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
        // dbg("delete count");
        delete count;
        delete ps;
        count = NULL;
    }
    // cout << "析构函数" << endl;
    dbg("析构函数");
}

void HasPtr::print() {
    dbg(count);
    // cout << "count= " << ps.use_count() << endl;
    // dbg(ps.use_count());
    dbg(*ps);
}