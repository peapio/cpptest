/*
1. 动态数组：使用动态数组来存储数据，以便职场在两端进行常数时间的插入和删除
2. 头尾指针：维护头尾的索引，以便快速访问两端
3. 自动货站
4. 迭代器支持
> 采用环形缓冲区，索引达到数组末端时，自动绕回数组的开头
*/
namespace my_std
{

template <typename T>
class Deque
{
private:
    T* buffer;          // 内部缓冲区
    size_t capacity;    // 缓冲区容量
    size_t front_idx;   // 头部索引
    size_t end_idx;     // 尾部索引
    size_t count;       // 当前元素数量

    void resize(size_t new_capacity)
    {
        T* new_buffer = new T[new_capacity];
        // 重新排列元素
        for (size_t i = 0; i < count; ++i) {
            new_buffer[i] = buffer[(front_idx + i) % capacity];
        }
        delete[] buffer;
        buffer = new_buffer;
        capacity = new_capacity;
        front_idx = 0;
        back_idx = count;
    }

public:
    Deque(size_t initial_capacity = 0)
    : capacity(initia_capacity), front_idx(0), end_idx(0), count(0) {}

    ~Deque()
    {
        delete[] buffer;
    }

    bool empty() const {
        return count == 0;
    }

    size_t size() const {
        return count;
    }

       // 在前面插入元素
    void push_front(const T& value) {
        if (count == capacity) {
            resize(capacity * 2);
        }
        front_idx = (front_idx == 0) ? capacity - 1 : front_idx - 1;
        buffer[front_idx] = value;
        ++count;
    }

    // 在后面插入元素
    void push_back(const T& value) {
        if (count == capacity) {
            resize(capacity * 2);
        }
        buffer[back_idx] = value;
        back_idx = (back_idx + 1) % capacity;
        ++count;
    }

    // 从前面删除元素
    void pop_front() {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        front_idx = (front_idx + 1) % capacity;
        --count;
    }

    // 从后面删除元素
    void pop_back() {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        back_idx = (back_idx == 0) ? capacity - 1 : back_idx - 1;
        --count;
    }

    // 获取前端元素
    const T& front() const {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        return buffer[front_idx];
    }

    // 获取后端元素
    const T& back() const {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        size_t last_idx = (back_idx == 0) ? capacity - 1 : back_idx - 1;
        return buffer[last_idx];
    }

};
} // namespace my_std
