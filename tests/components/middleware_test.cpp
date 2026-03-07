#include "../../include/components.hpp"

#include <any>
#include <vector>


/** @cond */
class test_middleware : public component::base_middleware {
    std::any process(std::vector<std::any>) override
        { return 0; }
};
/** @endcond */


extern "C" test_middleware *constructor() {
    return new test_middleware;
};
