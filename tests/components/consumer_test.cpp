#include "../../include/components.hpp"

#include <vector>


/** @cond */
class test_consumer : public component::base_consumer {
    void consume(std::vector<std::any>) override {}
};
/** @endcond */


extern "C" test_consumer *constructor() {
    return new test_consumer;
};
