#include "dbg.h"
#include "test/class_test.h"

#if 1
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
#endif
int main(int argc, char const *argv[])
{
    /* code */
    test_class();
    dbg("hello, world!!!");
    dbg("zxcv");
    return 0;
}

