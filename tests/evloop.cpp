#include "../include/evloop.hpp"
#include "../include/events.hpp"

#include <cassert>
#include <memory>
#include <unistd.h>


static bool event_run = false;

class test_event : public events::base_event {
    void operator()([[maybe_unused]] std::shared_ptr<evloop::evloop> e) override {
        event_run = true;
    }
};


int main()  {
    auto evloop = evloop::evloop::create();

    evloop->push_event(std::make_unique<test_event>());

    while (!event_run)
        ;

    evloop->shutdown();
}
