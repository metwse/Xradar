#include "../../include/components.hpp"
#include "../../include/config.hpp"

#include <memory>
#include <vector>


/** @cond */
class test_consumer : public component::base_consumer {
    void send(std::vector<std::shared_ptr<std::any>>) override {}

    void configure(config::config) override {}  // GCOVR_EXCL_LINE
};
/** @endcond */


extern "C" test_consumer *constructor() {
    return new test_consumer;
};
