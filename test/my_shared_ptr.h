
#include <atomic>

typedef struct ControlBlock{
    std::atomic<int> ref_count;
    ControlBlock(): ref_count(1){}
}CB;

template<typename T>
class MySharedPtr{
public:
    MySharedPtr();
    ~MySharedPtr();


    explicit MySharedPtr(T* ptr);
    MySharedPtr(const MySharedPtr<T>& other);
    MySharedPtr<T>& operator=(const MySharedPtr<T>& other);

    MySharedPtr(MySharedPtr<T>&& other);
    MySharedPtr<T>& operator=(MySharedPtr<T>&& other);

    int use_count() {return m_cb->ref_count;};

    // void reset() { release(); };

    T* get() {return m_ptr; };



private:

    void release();
    T * m_ptr;
    CB * m_cb;

};

template<typename T>
void MySharedPtr<T>::release()
{
    if(m_cb)
    {
        m_cb->ref_count.fetch_sub(1);
        if(m_cb->ref_count == 0)
        {
            delete m_ptr;
            delete m_cb;
        }
    }
    m_ptr = nullptr;
    m_cb = nullptr;
}


template<typename T>
MySharedPtr<T>::MySharedPtr(): m_ptr(nullptr), m_cb(nullptr) {}

template<typename T>
MySharedPtr<T>::MySharedPtr(T* ptr): m_ptr(nullptr), m_cb(nullptr)
{
    m_ptr = ptr;
    m_cb = new CB();

}

template<typename T>
MySharedPtr<T>::~MySharedPtr()
{
    release();
}

template<typename T>
MySharedPtr<T>::MySharedPtr(const MySharedPtr<T>& other)
{
    m_ptr = other.m_ptr;
    m_cb = other.m_cb;
    m_cb->ref_count.fetch_add(1);
}

template<typename T>
MySharedPtr<T>& MySharedPtr<T>::operator=(const MySharedPtr<T>& other)
{
    if(this != &other)
    {
        release();
        m_ptr = other.m_ptr;
        m_cb = other.m_cb;
        m_cb->ref_count.fetch_add(1);
    }
    return *this;
}

template<typename T>
MySharedPtr<T>::MySharedPtr(MySharedPtr<T>&& other)
{
    m_ptr = other.m_ptr;
    m_cb = other.m_cb;
    other.m_ptr = nullptr;
    other.m_cb = nullptr;
}

template<typename T>
MySharedPtr<T>& MySharedPtr<T>::operator=(MySharedPtr<T>&& other)
{
    if(this != &other)
    {
        release();
        m_ptr = other.m_ptr;
        m_cb = other.m_cb;
        other.m_ptr = nullptr;
        other.m_cb = nullptr;

    }
    return *this;
}
