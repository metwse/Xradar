#include "../include/components.hpp"
#include "../include/config.hpp"


/** @brief Mock signal source. */
class mock_producer : public component::base_producer {
    void start() override {}

    void stop() override {}

    void configure(config) override {}
};


extern "C" mock_producer *constructor() {
    return new mock_producer;
};
