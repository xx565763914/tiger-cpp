#pragma once
#include <mutex>
#include <condition_variable>
#include <list>
#include <iostream>
#include "log.hpp"

/**
 * @brief 这是一个阻塞队列
 * 
 */

 template<class T> class BlockingQueue {
     public:

        BlockingQueue() {
            // max_size为-1表示为一个无界阻塞队列
            this->max_size = -1;
        }

        BlockingQueue(int max_size) {
            this->max_size = max_size;
        }

        void put(const T& val) {
            std::lock_guard<std::mutex> locker(lock);
            while (isFull()) {
                LOG_WARN("阻塞队列已经满了，等待一会儿...");
                cvFull.wait(lock);
            }
            q.emplace_back(val);
            cvEmpty.notify_all();
        }

        T take() {
            std::lock_guard<std::mutex> locker(lock);
            while (isEmpty()) {
                LOG_WARN("阻塞队列为空，等待一会儿...");
                cvEmpty.wait(lock);
            }
            T val = q.front();
            q.pop_front();
            cvFull.notify_all();
            return val;
        }

    private:
        bool isEmpty() {
            return q.size() == 0;
        }

        bool isFull() {
            return max_size > 0 && q.size() == max_size;
        }

    private:
        std::mutex lock;
        std::condition_variable_any cvFull, cvEmpty;
        std::list<T> q;
        int size;
        int max_size;
 };
