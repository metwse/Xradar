#include "../include/components.hpp"
#include "../include/config.hpp"


/** @brief Built-in mock signal source. */
class mock : public component::base_producer {
    using base_producer::base_producer;

    void start() override {}

    void stop() override {}

    void configure(config) override {}
};


extern "C" mock *constructor(void *state_callback, void *data_callback) {
    return new mock { state_callback, data_callback };
};
