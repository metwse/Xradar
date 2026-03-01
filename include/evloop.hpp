/**
 * @file evloop.hpp
 * @brief Event loop.
 */

#ifndef EVLOOOP_HPP
#define EVLOOOP_HPP


#include <condition_variable>
#include <cstddef>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class state;  /* internal: defined in state.hpp */


namespace evloop {

class base_event;

/** @brief Event loop frontend. */
class evloop : std::enable_shared_from_this<evloop> {
private:
    struct token { explicit token() = default; };

public:
    /** @cond */
    evloop(token) {}
    /** @endcond */

    /** @brief Create the event loop. */
    static std::shared_ptr<evloop> create();

    /**
     * @brief Request the event loop to shutdown.
     *
     * This method should explicitly be called to destroy evloop as the workers
     * holds a reference to `this`.
     */
    void shutdown();

    /**
     * @brief Adds event(s) to the event loop.
     *
     * Events provided in a single call are guaranteed to be executed
     * sequentially in the order they are passed. However, the relative order
     * of events between separate calls to this function is not guaranteed.
     */
    template<typename... Args>
    void push_event(Args &&...args) {
        {
            std::lock_guard guard { event_queue_m };

            (event_queue.push_back(std::forward<Args>(args)), ...);
        }

        event_queue_cv.notify_one();
    }


    /** @brief Underlying application state. */
    std::shared_ptr<::state> state;

private:
    static void run(std::shared_ptr<evloop>, size_t);

    bool running { true };

    /* Reports the event loop thread is running. */
    std::condition_variable thread_ready_cv;
    std::mutex thread_ready_m;
    size_t thread_ready { 0 };

    std::condition_variable event_queue_cv;
    std::mutex event_queue_m;
    std::deque<std::unique_ptr<base_event>> event_queue;

    std::vector<std::thread> threads;
};

/** @brief Scheduled event. */
class base_event {
public:
    /** @brief Operation. */
    virtual void operator()(std::shared_ptr<evloop>) = 0;
};

}


#endif
