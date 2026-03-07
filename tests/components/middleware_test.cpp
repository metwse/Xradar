#include "../../include/components.hpp"

#include <any>
#include <memory>
#include <vector>


/** @cond */
class test_middleware : public component::base_middleware {
    std::unique_ptr<std::any> process(std::vector<std::shared_ptr<std::any>>) override
        { return std::make_unique<std::any>(); }
};
/** @endcond */


extern "C" test_middleware *constructor() {
    return new test_middleware;
};
