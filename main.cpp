#include "dbg.h"
#include <cassert>
#include <thread>
#include "test/class_test.h"
#include "test/my_func.h"
#include "test/my_string.h"
#include "test/my_shared_ptr.h"
#include "test/my_list.h"

#if 0
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

void test_myfunc()
{
    printf("strcmp(\"abc\", \"abc\") = %d\n", MyStrCmp("abc", "abc"));  // 0
    printf("strcmp(\"abc\", \"abd\") = %d\n", MyStrCmp("abc", "abd"));  // -1

    // 测试 strcpy
    char dest[20];
    printf("strcpy(dest, \"Hello\") = %s\n", MyStrCpy("Hello", dest));  // Hello

    // 测试 strcat
    MyStrCpy("Hello", dest);
    printf("strcat(dest, \" World\") = %s\n", MyStrCat(" World", dest));  // Hello World

}

void test_mystr()
{
    MyString AD;
    char * p="ABCDE";
    MyString B(p);
    // B.print();
    AD.print();
    AD=B;
    AD.print();
}


void test_shared_ptr()
{
    std::cout << "=== 开始测试 MySharedPtr ===" << std::endl;

    // 测试1: 基础构造和解引用
    {
        MySharedPtr<int> ptr1(new int(42));
        assert(*ptr1.get() == 42);  // 解引用测试
        std::cout << "✅ 基础构造和解引用测试通过" << std::endl;
    }  // ptr1 应在此析构

    // 测试2: 拷贝构造和引用计数
    {
        MySharedPtr<int> ptr1(new int(100));
        MySharedPtr<int> ptr2 = ptr1;  // 拷贝构造
        assert(*ptr2.get() == 100 && ptr1.use_count() == 2);
        std::cout << "✅ 拷贝构造和引用计数测试通过" << std::endl;
    }  // ptr1 和 ptr2 析构，引用计数归零

    // 测试3: 赋值操作
    {
        MySharedPtr<int> ptr1(new int(10));
        MySharedPtr<int> ptr2(new int(20));
        ptr2 = ptr1;  // 赋值操作
        assert(*ptr2.get() == 10 && ptr1.use_count() == 2);
        std::cout << "✅ 赋值操作测试通过" << std::endl;
    }

    // 测试4: 移动语义
    {
        MySharedPtr<int> ptr1(new int(30));
        MySharedPtr<int> ptr2 = std::move(ptr1);  // 移动构造
        assert(ptr1.get() == nullptr && *ptr2.get() == 30);
        std::cout << "✅ 移动语义测试通过" << std::endl;
    }

    // 测试5: 空指针处理
    {
        MySharedPtr<int> ptr(nullptr);
        assert(ptr.get() == nullptr);
        std::cout << "✅ 空指针处理测试通过" << std::endl;
    }

    std::cout << "=== 所有测试通过 ===" << std::endl;

}

#endif

void test_mylist() {
    std::cout << "=== 开始测试 ThreadSafeListQueue ===" << std::endl;

    MyList<int> queue;

    // 测试1：基础功能（单线程）
    {
        assert(queue.empty());  // 初始应为空
        queue.push(42);
        assert(!queue.empty());  // 插入后非空
        assert(queue.pop() == 42);  // 弹出值正确
        assert(queue.empty());  // 弹出后应为空
        std::cout << "✅ 基础功能测试通过" << std::endl;
    }

    // 测试2：多线程生产者-消费者
    {
        const int num_producers = 2;
        const int num_consumers = 2;
        const int items_per_producer = 100;
        std::vector<std::thread> producers;
        std::vector<std::thread> consumers;
        std::atomic<int> total_consumed(0);

        // 启动生产者线程
        for (int i = 0; i < num_producers; ++i) {
            producers.emplace_back([&queue, i] {
                for (int j = 0; j < items_per_producer; ++j) {
                    queue.push(i * 1000 + j);  // 生产数据
                }
            });
        }

        // 启动消费者线程
        for (int i = 0; i < num_consumers; ++i) {
            consumers.emplace_back([&queue, &total_consumed] {
                for (int j = 0; j < items_per_producer; ++j) {
                    int val = queue.pop();  // 消费数据
                    total_consumed += val;
                }
            });
        }

        // 等待所有线程完成
        for (auto& t : producers) t.join();
        for (auto& t : consumers) t.join();

        // 验证总数（生产者总和 = 消费者总和）
        int expected_sum = 0;
        for (int i = 0; i < num_producers; ++i) {
            for (int j = 0; j < items_per_producer; ++j) {
                expected_sum += i * 1000 + j;
            }
        }
        assert(total_consumed == expected_sum);
        std::cout << "✅ 多线程生产者-消费者测试通过" << std::endl;
    }


    std::cout << "=== 所有测试通过 ===" << std::endl;
}

int main(int argc, char const *argv[])
{
    /* code */
    // test_class();
    // test_mystr();
    // test_shared_ptr();
    dbg("code start");
    test_mylist();

    dbg("code end");
    // dbg("zxcv");
    return 0;
}

