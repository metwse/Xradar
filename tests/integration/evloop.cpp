#include "../../include/evloop.hpp"

#include <cassert>
#include <mutex>
#include <unistd.h>


static std::mutex event_run_m;
static int event_run = 0;

/** @brief Dummy event stting event_run variable true. */
class test_event {
public:
    void operator()() {
        std::lock_guard g { event_run_m };

        event_run++;
    }
};


int main()  {
    auto evloop = evloop::evloop::create();

    for (int i = 0; i < 10; i++)
        evloop->push_event(test_event {},
                           test_event {});

    while (event_run != 20)
        ;

    for (int i = 0; i < 10; i++)
        evloop->push_event(test_event {},
                           test_event {});

    evloop->shutdown();  /* shutdown joins all threads and waits until the
                          * event queue completed */

    evloop->shutdown();  /* second call to shutdown is ignored */

    assert(event_run == 40);
}
