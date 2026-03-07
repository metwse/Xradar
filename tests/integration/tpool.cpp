#include "../../include/tpool.hpp"

#include <cassert>
#include <mutex>
#include <unistd.h>


static int tasks_run = 0;

void test_task() {
    static std::mutex tasks_run_m;

    std::lock_guard g { tasks_run_m };

    tasks_run++;
}

int main()  {
    auto tpool = tpool::tpool::create();

    for (int i = 0; i < 10; i++)
        tpool->push_task(test_task, test_task);

    while (tasks_run != 20)
        ;

    for (int i = 0; i < 10; i++)
        tpool->push_task(test_task, test_task);

    tpool->shutdown();  /* `shutdown` joins all threads and waits until the
                         * task queue completed */

    tpool->shutdown();  /* second call to shutdown is ignored */

    assert(tasks_run == 40);
}
