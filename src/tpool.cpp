#include "../include/tpool.hpp"

#include <clocale>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>


std::shared_ptr<tpool::tpool> tpool::tpool::create() {
    size_t num_threads = std::thread::hardware_concurrency();

    if (num_threads == 0)
        num_threads = 1;  // GCOVR_EXCL_LINE

    auto shared_this = std::make_shared<tpool>(token {});

    shared_this->threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; i++)
        shared_this->threads.insert(shared_this->threads.end(),
                                    std::thread { run, shared_this, i });

    std::unique_lock lk { shared_this->thread_ready_m };
    shared_this->thread_ready_cv.wait(lk, [&shared_this, num_threads] (){
        return shared_this->thread_ready == num_threads;
    });

    return shared_this;
}

void tpool::tpool::shutdown() {
    {
        std::lock_guard guard { task_queue_m };

        if (!running)
            return;

        running = false;
    }

    task_queue_cv.notify_all();

    for (auto &thread : threads)
        thread.join();
}

void tpool::tpool::run(std::shared_ptr<tpool> e,
                       [[maybe_unused]] size_t thread_id  /* for debug */) {
    {
        std::lock_guard guard { e->thread_ready_m };

        e->thread_ready++;
    }

    e->thread_ready_cv.notify_one();

    while (e->running) {
        std::deque<std::function<void()>> tasks_to_process;

        {
            std::unique_lock lk { e->task_queue_m };
            e->task_queue_cv.wait(lk, [&e]() {
                return !e->task_queue.empty() || !e->running;
            });

            tasks_to_process = std::move(e->task_queue);
        }

        for (auto &task : tasks_to_process)
            task();
    }
}
