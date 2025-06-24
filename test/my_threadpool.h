#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class ThreadPool {
public:
    // 构造函数，启动线程池
    explicit ThreadPool(size_t threadCount = std::thread::hardware_concurrency()) 
        : stop(false) {
        for (size_t i = 0; i < threadCount; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        // 等待任务或停止信号
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { 
                            return stop || !tasks.empty(); 
                        });

                        // 收到停止信号且任务已执行完
                        if (stop && tasks.empty()) return;

                        // 取任务
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task(); // 执行任务
                }
            });
        }
    }

    // 添加任务（支持任意返回类型）
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;

        // 封装任务为shared_ptr以便传递
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one(); // 唤醒一个线程
        return res;
    }

    // 析构函数（安全关闭）
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all(); // 唤醒所有线程
        for (std::thread &worker : workers) {
            worker.join();
        }
    }

private:
    std::vector<std::thread> workers;    // 工作线程组
    std::queue<std::function<void()>> tasks; // 任务队列
    std::mutex queueMutex;              // 队列互斥锁
    std::condition_variable condition;  // 条件变量
    bool stop;                          // 停止标志
};
#if 0
int main() {
    ThreadPool pool(4); // 4个工作线程

    // 提交多个任务
    std::vector<std::future<int>> results;
    for (int i = 0; i < 8; ++i) {
        results.emplace_back(pool.enqueue([i] {
            std::cout << "Task " << i << " running\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return i * i;
        }));
    }

    // 获取结果
    for (auto&& result : results) {
        std::cout << "Result: " << result.get() << std::endl;
    }

    return 0;
}
#endif