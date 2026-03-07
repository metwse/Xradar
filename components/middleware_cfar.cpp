#include "../include/components.hpp"

#include <memory>
#include <vector>


/** @brief Built-in CFAR middleware. */
class cfar_middleware : public component::base_middleware {
    std::any process(std::vector<std::any>) override
        { throw "todo"; }
};


extern "C" cfar_middleware *constructor() {
    return new cfar_middleware;
};
