#include "../include/components.hpp"


/** @brief Consumer for testing. */
class test_consumer : public component::base_consumer {
    void send(std::shared_ptr<std::any>) override {}

    void configure(config) override {}
};


extern "C" test_consumer *constructor() {
    return new test_consumer;
};
