#include "../include/components.hpp"


/** @brief X11 graphical user interface. */
class x11 : public component::base_consumer {
    void send(std::shared_ptr<std::any>) override {}

    void configure(config) override {}
};


extern "C" x11 *constructor() {
    return new x11;
};
