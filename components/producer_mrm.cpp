#include "../include/components.hpp"
#include "../include/config.hpp"


/** @brief MRM signal source. */
class mrm_producer : public component::base_producer {
    void start() override {}

    void stop() override {}

    void configure(config::config) override {}
};


extern "C" mrm_producer *constructor() {
    return new mrm_producer;
};
