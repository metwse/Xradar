#include "../../include/components.hpp"
#include "../../include/config.hpp"

#include <memory>


class test_middleware : public component::base_middleware {
    void feed(std::shared_ptr<std::any>) override {};

    void configure(config::config) override {}  // GCOVR_EXCL_LINE
};


extern "C" test_middleware *constructor() {
    return new test_middleware;
};
