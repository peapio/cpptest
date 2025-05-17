
#include <iostream>
#include <cstdio>
#include <vector>
#include <list>
#include <memory>
#include "../dbg.h"

typedef std::shared_ptr<int> SharedPtr;
typedef std::unique_ptr<int> UniquePtr;
typedef std::weak_ptr<int> WeakPtr;


class HasPtr {
public:

    // HasPtr();
    HasPtr(const std::string &s);
    HasPtr(const HasPtr& other);
    HasPtr& operator=(const HasPtr& other);
    ~HasPtr();
    void print();
private:
    int* count;
    std::string* ps;
};