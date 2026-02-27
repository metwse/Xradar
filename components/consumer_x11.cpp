#include "../include/components.hpp"
#include "../include/config.hpp"


/** @brief X11 graphical user interface. */
class x11 : public component::base_consumer {
    void send(std::shared_ptr<std::any>) override {}

    void configure(config::config) override {}
};


extern "C" x11 *constructor() {
    return new x11;
};
