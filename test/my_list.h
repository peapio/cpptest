
#include <list>
#include <mutex>
#include <condition_variable>

template <typename T>
class MyList
{
public:
    // MyList();
    // ~MyList();
    // MyList(const MyList<T>& other);
    // MyList<T>& operator=(const MyList<T>& other);

    // MyList(MyList<T>&& other);
    // MyList<T>& operator=(MyList<T>&& other);

    void push(const T& item);

    T pop();


    bool empty() const;

private:

    mutable std::mutex m_mtx;
    std::list<T> m_lst;
    std::condition_variable m_cond;

};

template<typename T>
void MyList<T>::push(const T& item)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    m_lst.push_back(item);
    m_cond.notify_one();

}

template<typename T>
T MyList<T>::pop()
{
    std::unique_lock<std::mutex> lock(m_mtx);
    m_cond.wait(lock, [this](){
        return !m_lst.empty();
    });
    T item = std::move(m_lst.front());
    m_lst.pop_front();
    return item;
}


template<typename T>
bool MyList<T>::empty() const
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_lst.empty();

}