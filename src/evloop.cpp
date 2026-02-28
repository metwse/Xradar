#include "../include/evloop.hpp"
#include "../include/events.hpp"

#include <memory>
#include <mutex>
#include <thread>


void evloop::evloop::shutdown() {
    {
        std::lock_guard guard { event_queue_m };

        if (!running)
            return;

        running = false;
    }

    event_queue_cv.notify_one();

    thread.join();
}

void evloop::evloop::push_event(std::unique_ptr<events::base_event> event) {
    {
        std::lock_guard guard { event_queue_m };

        event_queue.push_back(std::move(event));
    }

    event_queue_cv.notify_one();
}

void evloop::evloop::run(std::shared_ptr<evloop> e) {
    {
        std::lock_guard guard { e->thread_ready_m };

        e->thread_ready = true;
    }

    e->thread_ready_cv.notify_one();

    while (true) {
        std::deque<std::unique_ptr<events::base_event>> items_to_process;

        {
            std::unique_lock lk { e->event_queue_m };
            e->event_queue_cv.wait(lk, [&e]() {
                return !e->event_queue.empty() || !e->running;
            });

            if (!e->running && e->event_queue.empty())
                break;

            items_to_process.swap(e->event_queue);
        }

        for (; !items_to_process.empty(); items_to_process.pop_front()) {
            auto &event = items_to_process.front();

            (*event)(e);
        }
    }
}
