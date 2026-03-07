#include "../include/components.hpp"

#include <memory>
#include <vector>


/** @brief X11 graphical user interface. */
class x11 : public component::base_consumer {
    void send(std::vector<std::shared_ptr<std::any>>) override {}
};


extern "C" x11 *constructor() {
    return new x11;
};
