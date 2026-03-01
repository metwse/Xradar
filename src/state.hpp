#ifndef STATE_HPP
#define STATE_HPP

#include <mutex>


/** @brief Application state. */
class state {
    /** @brief Mutual exclusion of application state. */
    std::mutex m;
};


#endif
