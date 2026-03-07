#include "../include/components.hpp"

#include <vector>


/** @brief X11 graphical user interface. */
class x11 : public component::base_consumer {
    void consume(std::vector<std::any>) override {}
};


extern "C" x11 *constructor() {
    return new x11;
};
