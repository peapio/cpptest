#include <mutex>

namespace my_std{

template <typename T>
struct defaultDeleter {
    void operator() (const T *ptr)
    {
        if(ptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }
};

template <typename T, typename Deleter = defaultDeleter<T>>
class UniquePtr{

private:
    T* ptr;
    Deleter deleter;
    mutable std::mutex mtx;

public:

    explicit UniquePtr(T* p = nullptr, const Deleter& d = Deleter())
    : ptr(p), deleter(d) {}

    // 禁止拷贝
    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr& operator=(const UniquePtr& other) = delete;

    UniquePtr(const UniquePtr&& other) noexcept
    {
        std::lock_guard<std::mutex> lock(other.mtx);
        ptr = other.ptr;
        deleter = std::move(other.deleter);
        other.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        if(this != &other)
        {
            std::scoped_lock lock(mtx, other.mtx);
            deleter(ptr);
            ptr = other.ptr;
            deleter = std::move(other.deleter);
            other.ptr = nullptr;
        }
        return *this;
    }

    ~UniquePtr()
    {
        std::lock_guard<std::mutex> lock(mtx);
        deleter(ptr);
    }

    T* release() noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T* p = nullptr) noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        delete ptr;
        ptr = p;
    }

    void swap(UniquePtr& other) noexcept
    {
        if (this != &other) {
            // 锁定两个互斥量
            std::scoped_lock lock(mtx, other.mtx);

            std::swap(ptr, other.ptr);
            std::swap(deleter, other.deleter);
        }
    }

    T& operator*() const noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        return *ptr;
    }

    T* operator->() const noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        return ptr;
    }

    T* get() const noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        return ptr;
    }

    explicit operator bool() const noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        return ptr != nullptr;
    }
};

}