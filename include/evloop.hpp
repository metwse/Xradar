/**
 * @file evloop.hpp
 * @brief Event loop.
 */

#ifndef EVLOOOP_HPP
#define EVLOOOP_HPP


#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>

namespace events { class base_event; };


namespace evloop {

/** @brief Event loop frontend. */
class evloop : std::enable_shared_from_this<evloop> {
private:
    struct token { explicit token() = default; };

public:
    /** @cond */
    evloop(token) {}
    /** @endcond */

    /** @brief Create the event loop. */
    static std::shared_ptr<evloop> create() {
        auto shared_this = std::make_shared<evloop>(token {});

        shared_this->thread = std::jthread { run, shared_this };

        std::unique_lock lk { shared_this->thread_ready_m };
        shared_this->thread_ready_cv.wait(lk, [&shared_this] (){
            return shared_this->thread_ready;
        });

        return shared_this;
    }

    /** @brief Request the event loop to shutdown. */
    void shutdown();

    /** @brief Add event to the loop. */
    void push_event(std::unique_ptr<events::base_event>);

private:
    static void run(std::shared_ptr<evloop>);

    bool running { true };

    std::condition_variable thread_ready_cv;
    std::mutex thread_ready_m;
    bool thread_ready { false };

    std::condition_variable event_queue_cv;
    std::mutex event_queue_m;
    std::deque<std::unique_ptr<events::base_event>> event_queue;

    std::jthread thread;
};

}


#endif
