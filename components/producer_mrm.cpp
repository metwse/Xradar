#include "../include/components.hpp"


/** @brief MRM signal source. */
class mrm_producer : public component::base_producer {
    void start() override {}

    void stop() override {}
};


extern "C" mrm_producer *constructor() {
    return new mrm_producer;
};
