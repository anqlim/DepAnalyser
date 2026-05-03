#include "thread_pool.h"

namespace DepAnalyser::Multithreading {

    ThreadPool::ThreadPool(size_t thread_count) {
        for (size_t i = 0; i < thread_count; i++) {
            threads_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex_);
                        // поток спит пока нет задач или не остановлен
                        cv_.wait(lock, [this] {
                            return stopped_ || !tasks_.empty();
                        });
                        if (stopped_ && tasks_.empty()) return;
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();
                }
            });
        }
    }

    ThreadPool::~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stopped_ = true;
        }
        cv_.notify_all(); // будим все потоки чтобы они завершились
        for (auto& t : threads_) t.join();
    }

    std::future<Types::ParseResult> ThreadPool::submit(std::function<Types::ParseResult()> task) {
        auto promise = std::make_shared<std::promise<Types::ParseResult>>();
        std::future<Types::ParseResult> future = promise->get_future();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.push([promise, task] {
                promise->set_value(task());
            });
        }
        cv_.notify_one();
        return future;
    }

}