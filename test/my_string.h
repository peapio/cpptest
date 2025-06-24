
#include <iostream>
#include <cstring>

class MyString
{
public:
    MyString(const char * str = nullptr);
    ~MyString();
    MyString(const MyString& other);
    // MyString(const MyString&& other);
    MyString& operator=(const MyString& other);
    int size() const {return m_len; };
    void print();


private:

    char * m_data;
    int m_len;

};

MyString::MyString(const char* str)
{
    if(str == nullptr)
    {
        m_data = new char[1];
        *m_data = '\0';
        m_len = 0;
    }
    else
    {
        m_len = strlen(str);
        m_data = new char[m_len + 1];
        strcpy(m_data, str);
    }
}

MyString::~MyString()
{
    delete [] m_data;
}

MyString::MyString(const MyString& other)
{
    m_data = new char[other.size()+1];
    strcpy(m_data, other.m_data);
}

MyString& MyString::operator=(const MyString& other)
{
    if(this != &other)
    {
        delete [] m_data;
        m_data = new char[other.size()+1];
        strcpy(m_data, other.m_data);
    }
    return *this;
}

void MyString::print()
{
    std::cout<<m_data<<std::endl;
}