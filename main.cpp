#include "class_test.h"

void test_class()
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

int main(int argc, char const *argv[])
{
    dbg("hello, world!!!");
    return 0;
}
