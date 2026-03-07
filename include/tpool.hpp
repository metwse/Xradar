/**
 * @file tpool.hpp
 * @brief Thread pool.
 */

#ifndef TPOOL_HPP
#define TPOOL_HPP


#include <condition_variable>
#include <cstddef>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class state;  /* internal: defined in state.hpp */


namespace tpool {

/** @brief Thread pool frontend. */
class tpool : public std::enable_shared_from_this<tpool> {
private:
    struct token { explicit token() = default; };

public:
    /** @cond */
    tpool(token) {}
    /** @endcond */

    /** @brief Create the thread pool. */
    static std::shared_ptr<tpool> create();

    /**
     * @brief Request the thread pool to shutdown.
     *
     * This method should explicitly be called to destroy tpool as the workers
     * holds a reference to `this`.
     */
    void shutdown();

    /**
     * @brief Submit task(s) to the pool.
     *
     * Tasks provided in a single call are guaranteed to be executed
     * sequentially in the order they are passed. However, the relative order
     * of tasks between separate calls to this function is not guaranteed.
     */
    template<typename... Args>
    void push_task(Args &&...args) {
        {
            std::lock_guard guard { task_queue_m };

            (task_queue.push_back(std::forward<Args>(args)), ...);
        }

        task_queue_cv.notify_one();
    }


    /** @brief Underlying application state. */
    std::shared_ptr<::state> state;

private:
    static void run(std::shared_ptr<tpool>, size_t);

    bool running { true };

    /* Used for awaiting all threads to be ready. */
    std::condition_variable thread_ready_cv;
    std::mutex thread_ready_m;
    size_t thread_ready { 0 };

    std::condition_variable task_queue_cv;
    std::mutex task_queue_m;
    std::deque<std::function<void()>> task_queue;

    std::vector<std::thread> threads;
};

}


#endif
