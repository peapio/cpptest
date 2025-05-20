#include <atomic>
#include <mutex>
#include "dbg.h"

namespace my_std{

struct ControlBlock {
    std::atomic<size_t> ref_count;
    ControlBlock() : ref_count(1) {}
};


template <typename T>
class SharedPtr {
private:
    T* ptr;
    ControlBlock* control;
    mutable std::mutex mtx;

    // 引用计数减一的操作
    void release()
    {
        if (control != nullptr)
        {
            if(control->ref_count.fetch_sub(1) == 1)
            {
                delete ptr;
                delete control;
                dbg("Resource and ControlBlock destroyed.");
            }
            else
            {
                dbg("Decremented ref_count to ", control->ref_count.load())
            }
        }
        ptr = nullptr;
        control = nullptr;
    }
    
public:
    SharedPtr(): ptr(nullptr), control(nullptr)
    {
        dbg("Default constructed ThreadSafeSharedPtr")
    }

    explicit SharedPtr(T* p): ptr(p)
    {
        if (p)
        {
            control = new ControlBlock()
            dbg("Constructed ThreadSafeSharedPtr, ref_count = ", control->ref_count.load())
        }
        else 
        {
            control = nullptr;
        }
    }

    ~SharedPtr()
    {
        release();
    }
    // 拷贝构造, 未持有资源，用另一个对象资源初始化本对象
    SharedPtr(const SharedPtr &other)
    {
        std::lock_guard<std::mutex> lock(other.mtx);
        ptr = other.ptr;
        control = other.control;
        if (control)
        {
            control->ref_count.fetch_add(1);
            dbg("Copied ThreadSafeSharedPtr, ref_count = ", control->ref_count.load())
        }
    }
    // 拷贝赋值运算符, 可能已持有资源，用另一个对象资源覆盖本对象资源
    SharedPtr& operator=(const SharedPtr* other)
    {
        if(this!= &other)
        {
            std::scoped_lock lock(mtx, other.mtx);
            release();
            ptr = other.ptr;
            control = other.control;
            if (control) {
                control->ref_count.fetch_add(1);
                std::cout << "Assigned ThreadSafeSharedPtr, ref_count = " << control->ref_count.load() << std::endl;
            }
        }
        return *this;
    }
    // 移动构造
    SharedPtr(SharedPtr &&other) noexcept
    {
        std::lock_guard<std::mutex> lock(other.mtx);
        // 此处需要锁other.mtx
        // 为什么不锁this.mtx?
        //  此时this.mtx还没构造，此时锁定没有意义
        // 为什么锁other.mtx?
        // 假设一个线程正在进行移动构造`SharedPtr newPtr(std::move(oldPtr))
        // 另一个线程正在通过 oldPtr 访问或修改数据（例如调用 operator* 或 reset）
        // 此时需要保护other的资源，否则可能出现资源抢占
        ptr = other.ptr;
        control = other.control;
        other.ptr = nullptr;
        other.control = nullptr;
        dbg("Moved ThreadSafeSharedPtr.");
    }
    // 移动赋值运算符
    SharedPtr& operator=(SharedPtr&& other) noexcept
    {
        if (this != &other)
        {
            // 为避免死锁，使用 std::scoped_lock 同时锁定两个互斥锁
            std::scoped_lock lock(mtx, other.mtx);
            release();
            ptr = other.ptr;
            control = other.control;
            other.ptr = nullptr;
            other.control = nullptr;
            std::cout << "Move-assigned ThreadSafeSharedPtr." << std::endl;
        }
        return *this;
    }

    // 解引用操作
    T& operator*() const 
    {
        std::lock_guard<std::mutex> lock(mtx);
        return *ptr;
    }
    // 箭头操作
    T* operator->() const 
    {
        std::lock_guard<std::mutex> lock(mtx);
        return ptr;
    }
    // 获取引用计数
    int use_count() const 
    {
        std::lock_guard<std::mutex> lock(mtx);
        return control ? control->ref_count.load() : 0;
    }

    T* get() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        return ptr;
    }
    // 重置指针
    void reset(T* p = nullptr)
    {
        std::lock_guard<std::mutex> lock(mtx);
        release();
        ptr = p;
        if (p)
        {
            control = new ControlBlock();
            std::cout << "Reset ThreadSafeSharedPtr, ref_count = " << control->ref_count.load() << std::endl;
        }
        else
        {
            control = nullptr;
        }
    }
};


}