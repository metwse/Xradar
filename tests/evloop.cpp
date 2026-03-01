#include "../include/evloop.hpp"

#include <cassert>
#include <memory>
#include <mutex>
#include <unistd.h>


static std::mutex event_run_m;
static int event_run = 0;

/** @brief Dummy event stting event_run variable true. */
class test_event : public evloop::base_event {
    void operator()([[maybe_unused]] std::shared_ptr<evloop::evloop> e) override {
        std::lock_guard g { event_run_m };

        event_run++;
    }
};


int main()  {
    auto evloop = evloop::evloop::create();

    for (int i = 0; i < 10; i++)
        evloop->push_event(std::make_unique<test_event>(),
                           std::make_unique<test_event>());

    while (event_run != 20)
        ;

    for (int i = 0; i < 10; i++)
        evloop->push_event(std::make_unique<test_event>(),
                           std::make_unique<test_event>());

    evloop->shutdown();

    assert(event_run == 40);
}
