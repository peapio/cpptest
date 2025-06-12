#include <thread>
#include <mutex>
#include <iostream>
/**
 * 如何实现在两个线程中交替打印1和2
 */

/**
 * 只使用互斥量
 * 全局变量num表示哪个线程输出
 * mutex控制两个线程访问num
 * 问题：
 *  1. 线程在不满足条件时，会反复获取锁，检查条件，释放锁，及时当前没有工作需要做
 *  浪费了CPU资源，这种轮询的方式就是忙等待，效率极低
 *  2. 有可能存在竞争条件风险，使得得不到想要的结果
 *  检查和修改不是原子的，虽然if(num==1)和num++受到锁保护，但是continue后，线程会立即释放锁并重新进入循环
 *  
 * 
 */
void PoorImpleman() {
    std::thread t1([]() {
        for (;;) {

            {
                std::lock_guard<std::mutex> lock(mtx_num);
                if (num == 1) {
                    std::cout << "thread A print 1....." << std::endl;
                    num++;
                    continue;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

        }

     });

    std::thread t2([]() {
        for (;;) {

            {
                std::lock_guard<std::mutex> lock(mtx_num);
                if (num == 2) {
                    std::cout << "thread B print 2....." << std::endl;
                    num--;
                    continue;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

        }

        });

    t1.join();
    t2.join();
}

void ResonableImplemention() {
    std::thread t1([]() {
        for (;;) {

            std::unique_lock<std::mutex> lock(mtx_num);
            cvA.wait(lock, []() {
                return num == 1;
                });

            num++;
            std::cout << "thread A print 1....." << std::endl;
            cvB.notify_one();
        }

        });

    std::thread t2([]() {
        for (;;) {

            std::unique_lock<std::mutex> lock(mtx_num);
            cvB.wait(lock, []() {
                return num == 2;
                });

            num--;
            std::cout << "thread B print 2....." << std::endl;
            cvA.notify_one();
        }

        });

    t1.join();
    t2.join();
}