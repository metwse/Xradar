#include "../../include/components.hpp"


/** @cond */
class test_producer : public component::base_producer {
    void start() override {}

    void stop() override {}
};
/** @endcond */


extern "C" test_producer *constructor() {
    return new test_producer;
};
