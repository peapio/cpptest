#include <mutex>
#include <memory>
#include <iostream>
namespace my_std
{

/*
# 静态局部变量
> 此版本适用于C++11以后，能保证线程安全
函数中定义一个静态局部变量，那么这个局部变量只会初始化一次
就是在这个函数第一次调用的时候，
以后无论调用几次这个函数，函数内静态变量都不再初始化
此版本单例模式在C++11以前存在多线程不安全情况，编译器可能会初始化多个静态变量

*/

class Single {
private:
    Single(){}
    Single(const Single&) = delete;
    Single& operator=(const Single&) = delete;
public:
    static Single& getInst()
    {
        static Single single;
        return single;
    }
};

/*
# 静态成员变量指针（饿汉式）
> 适用于C++11以前
主线程启动后，其他线程没有启动前（作为全局变量初始化单例）
程序启动时就进行单例的初始化，由主线程先初始化单例资源。
这样其他线程获取资源就不涉及重复初始化的情况了。
饿汉式是从使用角度规避多线程的安全问题，
很多情况下很难从规则角度限制开发人员，所以这种方式不推荐
*/

class Single2Hungry
{
private:
    Single2Hungry() {}
    Single2Hungry(const Single2Hungry&) = delete;
    Single2Hungry& operator=(const Single2Hungry&) = delete;
public:
    static Single2Hungry *getInst()
    {
        if(single == nullptr)
        {
            single = new Single2Hungry();
        }
        return single;
    }
private:
    static Single2Hungry* single;
};

// 调用方式如下
#if 0
Single2Hungry *Single2Hungry::single = Single2Hungry::GetInst();
void thread_func_s2(int i)
{
    cout << "this is thread " << i << endl;
    cout << "inst is " << Single2Hungry::GetInst() << endl;
}
void test_single2hungry()
{
    cout << "s1 addr is " << Single2Hungry::GetInst() << endl;
    cout << "s2 addr is " << Single2Hungry::GetInst() << endl;
    for (int i = 0; i < 3; i++)
    {
        thread tid(thread_func_s2, i);
        tid.join();
    }
}
int main(){
    test_single2hungry()
}
#endif
/*
# 静态成员变量指针（懒汉式）
> 适用于C++11以前
用到时如果没有初始化单例则初始化，否则直接使用
这种情况需要加锁，防止资源被重复初始化
这种方式存在很严重的问题
多个线程都调用单例函数时，不确定资源是被哪个线程初始化的
回收指针存在问题，存在多重释放或不知道哪个指针释放的问题
*/

class SinglePointer
{
private:
    SinglePointer() {}
    SinglePointer(const SinglePointer&) = delete;
    SinglePointer& operator=(const Single2Hungry&) = delete;
public:
    // 多个线程可能同时调用getInst
    // 为什么要判断single != nullptr两次？
    // 第一次检查不加锁，避免锁的性能开销
    //  大多数情况下，单例已经创建，无需加锁
    // 第二次检查防止在第一次检查后，第二次检查前，
    // 另一个线程已经创建了单例
    // 为什么要加锁？
    // 如果不加锁，可以会被多个线程new多次，这样多个线程拿到的不是同一个地址，破坏单例唯一性不说，后续使用内存还会失效
    static SinglePointer* getInst()
    {
        if(single != nullptr)
        {
            return single;
        }
        // 加入两个线程都同时到达lock处，第一个线程先获得锁，那么第一个线程就先初始化了单例，所以加锁后需要再判断一次
        s_mutex.lock();
        if(single != nullptr)
        {
            s_mutex.unlock();
            return single;
        }
        single = new SinglePointer();
        s_mutex.unlock();
        return single;
    }
private:
    static SinglePointer* single;
    static std::mutex s_mutex;
};
// 调用方式如下
#if 0
SinglePointer* SinglePointer::single = nullptr;
std::mutex SinglePointer::s_mutex;
void thread_func_lazy(int i)
{
    std::cout << "this is lazy thread " << i << std::endl;
    std::cout << "inst is " << SinglePointer::GetInst() << std::endl;
}
void test_singlelazy()
{
    for (int i = 0; i < 3; i++)
    {
        std::thread tid(thread_func_lazy, i);
        tid.join();
    }
    //何时释放new的对象？造成内存泄漏
}
#endif

/**
 * # 智能指针方式（懒汉式）
 * 自动初始化并回收资源
 * 都有智能指针了，还是局部静态变量吧
 * 就是懒汉式静态成员变量指针改为了静态成员变量智能指针
 */

class SingleAuto
{
private:
    SingleAuto() {};
    SingleAuto(const SingleAuto&) = delete;
    SingleAuto& operator=(const SingleAuto&) = delete;
public:
    static std::shared_ptr<SingleAuto> getInst()
    {
        if(single != nullptr)
        {
            return single;
        }
        // 加入两个线程都同时到达lock处，第一个线程先获得锁，那么第一个线程就先初始化了单例，所以加锁后需要再判断一次
        s_mutex.lock();
        if(single != nullptr)
        {
            s_mutex.unlock();
            return single;
        }
        single = std::shared_ptr<SingleAuto>(new SingleAuto);
        s_mutex.unlock();
        return single;

    }
private:
    static std::shared_ptr<SingleAuto> single;
    static std::mutex s_mutex;
};

// 调用方式如下
#if 0
std::shared_ptr<SingleAuto> SingleAuto::single = nullptr;
std::mutex SingleAuto::s_mutex;
void test_singleauto()
{
    auto sp1 = SingleAuto::GetInst();
    auto sp2 = SingleAuto::GetInst();
    std::cout << "sp1  is  " << sp1 << std::endl;
    std::cout << "sp2  is  " << sp2 << std::endl;
    //此时存在隐患，可以手动删除裸指针，造成崩溃
    // delete sp1.get();
}
#endif

/**
 * 适合C++11以上的单例模板类
 */
template <typename T>
class MeyerSingleton {
public:
    static T& getInstance() {
        static T instance;
        return instance;
    }
    
    MeyerSingleton(const MeyerSingleton&) = delete;
    MeyerSingleton& operator=(const MeyerSingleton&) = delete;
    
protected:
    MeyerSingleton() = default;
    virtual ~MeyerSingleton() = default;
};
/**
 * 单例模板类
 */
template <typename T>
class Singleton {
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator=(const Singleton<T>& st) = delete;

    static std::shared_ptr<T> _instance;
public:
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() {
            _instance = shared_ptr<T>(new T);
        });

        return _instance;
    }
    void PrintAddress() {
        std::cout << _instance.get() << endl;
    }
    ~Singleton() {
        std::cout << "this is singleton destruct" << std::endl;
    }
};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

} // namespace my_std
