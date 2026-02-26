#include "../include/components.hpp"


/** @brief Built-in X11 interface (consumer). */
class x11 : public component::base_consumer {
    using base_consumer::base_consumer;

    void send(std::shared_ptr<std::any>) override {}

    void configure(config) override {}
};


extern "C" x11 *constructor() {
    return new x11 {};
};
