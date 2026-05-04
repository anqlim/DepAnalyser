#pragma once
#include "../types.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <queue>

namespace DepAnalyser::Multithreading {

    class ThreadPool {
    public:
        explicit ThreadPool(size_t thread_count);
        ~ThreadPool();
        std::future<Types::ParseResult> submit(std::function<Types::ParseResult()> task);

    private:
        std::vector<std::thread> threads_;
        std::queue<std::function<void()>> tasks_;
        std::mutex mutex_;
        std::condition_variable cv_;
        bool stopped_ = false;
    };

}
