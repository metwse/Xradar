#include "../../include/components.hpp"
#include "../../include/config.hpp"


class test_producer : public component::base_producer {
    void start() override {}

    void stop() override {}

    void configure(config::config) override {}  // GCOVR_EXCL_LINE
};


extern "C" test_producer *constructor() {
    return new test_producer;
};
