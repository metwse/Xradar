#include "../../include/components.hpp"
#include "../../include/config.hpp"


class test_consumer : public component::base_consumer {
    void send(std::shared_ptr<std::any>) override {}

    void configure(config::config) override {}  // GCOVR_EXCL_LINE
};


extern "C" test_consumer *constructor() {
    return new test_consumer;
};
