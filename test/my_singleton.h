#include <mutex>

class MySingleton{
private:
    MySingleton() {}
    MySingleton(const MySingleton& other) = delete;
    MySingleton& operator=(const MySingleton& other) = delete;
public:
    static MySingleton& getInst()
    {
        static MySingleton singleton;
        return singleton;
    }


};

class MySingletonPtr{
private:
    MySingletonPtr() {}
    MySingletonPtr(const MySingletonPtr& other) = delete;
    MySingletonPtr& operator=(const MySingletonPtr& other) = delete;
public:
    static MySingletonPtr* getInst()
    {
        if(m_singleton != nullptr)
        {
            return m_singleton;
        }
        m_mtx.lock();
        if(m_singleton != nullptr)
        {
            m_mtx.unlock();
            return m_singleton;
        }
        m_singleton = new MySingletonPtr();
        m_mtx.unlock();
        return m_singleton;
    }
private:
    static MySingletonPtr * m_singleton;
    static std::mutex m_mtx;

};