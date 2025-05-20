#include <atomic>
#include <memory>

namespace my_std
{
// 与shared_ptr共用一个控制块
template <typename T>
struct ControlBlock {
    std::atomic<size_t> ref_count{1};
    std::atomic<size_t> weak_count{0};
    // 用于跟踪有多少个weak_ptr正在观察这个控制块

    T* ptr = nullptr;
    explicit ControlBlock(T* p) : ptr(p) {}

    void release_shared()
    {
        if(ref_count.fetch_sub(1) == 1)
        {
            delete ptr;
            ptr = nullptr;
            if (weak_count.load() == 0)
            {
                delete this;
            }
        }
    }

    void release_weak()
    {
        if (weak_count.fetch_sub(1) == 1)
        {
            if(ref_count.load() == 0)
            {
                delete this;
            }
        }
    }
};

template <typename T>
class SharedPtr {
private:
    T* ptr;
    ControlBlock<T>* control;
    mutable std::mutex mtx;

public:
    SharedPtr(): ptr(nullptr), control(nullptr)
    {
        dbg("Default constructed ThreadSafeSharedPtr")
    }

    explicit SharedPtr(T* p): ptr(p)
    {
        if (p)
        {
            control = new ControlBlock<T>(p);
            dbg("Constructed ThreadSafeSharedPtr, ref_count = ", control->ref_count.load())
        }
        else 
        {
            control = nullptr;
        }
    }

    ~SharedPtr()
    {
        if(control) control->release_shared();
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
};


}

template <typename T>
class WeakPtr{

private:
    ControlBlock* control nullptr;

    void release()
    {
        if(control)
        {
            if(--(control->weak_count) == 0)
            {
                delete control;
            }
        }
    }

public:
    // 默认构造
    WeakPtr() noexcept = default;

    // 用shared_ptr构造，观察shared_ptr，weak_ptr加一，观察对象多一个
    WeakPtr(const SharedPtr<T>& sp) noexcept
    {
        control = sp.control;
        if(control)
            control->weak_count.fetch_add(1);
    }
    //拷贝构造, 拷贝weak_ptr时weak_count加一，表示观察对象多一个
    WeakPtr(const WeakPtr& other) noexcept
    {
        control = other.control;
        if(control)
            control->weak_count--;
        
    }

    // 拷贝赋值, 拷贝赋值时可能带有资源，所以释放原有的，拷贝新来的
    WeakPtr& operator=(const WeakPtr& other) noexcept {
        if (this != &other) {
            if (control) control->release_weak();
            control = other.control;
            if (control) {
                control->weak_count.fetch_add(1);
            }
        }
        return *this;
    }

    // 移动构造
    WeakPtr(const WeakPtr&& other) noexcept
    {
        control = other.control;
        other.control = nullptr;
    }

    // 移动赋值
    WeakPtr& operator=(weak_ptr&& other) noexcept {
        if (this != &other) {
            if(control) control->release_weak();
            control = other.control;
            other.control = nullptr;
        }
        return *this;
    }

    ~WeakPtr()
    {
        release();
    }
     // 从 shared_ptr 赋值，放弃旧的观察对象，转为新的观察对象
    template <typename U>
    WeakPtr& operator=(const shared_ptr<U>& other) noexcept {
        if(control) control->release_weak();
        control = other.control;
        if (control) {
            control->weak_count.fetch_add(1);
        }
        return *this;
    }

    // 尝试获取一个shared_ptr，如果所指对象已被销毁，返回空指针???
    // 学到内存序再了解吧
    shared_ptr<T> lock() const noexcept {
        // 如果已销毁，不管了 直接返回空指针
        if (expired()) {
            return shared_ptr<T>();
        }
        
        // 如果没销毁，尝试增加 shared_count
        while (true) {
            size_t old_count = control->ref_count.load();
            if (old_count == 0) {
                return shared_ptr<T>();
            }
            
            if (control->ref_count.compare_exchange_weak(
                old_count, old_count + 1, 
                std::memory_order_acq_rel, std::memory_order_relaxed)) {
                return shared_ptr<T>(ptr, control);
            }
        }
    }

    // 检查shared_ptr是否被销毁
    bool expired() const noexcept {
        return !control || control->shared_count.load() == 0;
    }

    void reset() noexcept {
        if(control) control->release_weak();
        control = nullptr;
    }

    // 交换两个 weak_ptr
    void swap(weak_ptr& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(control, other.control);
    }

    // 获取引用计数（主要用于调试）
    size_t use_count() const noexcept {
        return control ? control->shared_count.load(std::memory_order_relaxed) : 0;
    }
};

} // namespace my_std
