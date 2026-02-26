#include "../include/components.hpp"
#include "../include/config.hpp"


/** @brief Built-in test signal source. */
class test_producer : public component::base_producer {
    void start() override {}

    void stop() override {}

    void configure(config) override {}
};


extern "C" test_producer *constructor() {
    return new test_producer;
};
