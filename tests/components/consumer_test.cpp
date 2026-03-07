#include "../../include/components.hpp"

#include <memory>
#include <vector>


/** @cond */
class test_consumer : public component::base_consumer {
    void send(std::vector<std::shared_ptr<std::any>>) override {}
};
/** @endcond */


extern "C" test_consumer *constructor() {
    return new test_consumer;
};
