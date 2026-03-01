#include "../include/components.hpp"
#include "../include/config.hpp"

#include <memory>


/** @brief Built-in CFAR middleware. */
class cfar_middleware : public component::base_middleware {
    std::shared_ptr<std::any> process(std::shared_ptr<std::any>) override
        { throw "todo"; }

    bool parallelizable() const override
        { return true; }

    void configure(config::config) override {}
};


extern "C" cfar_middleware *constructor() {
    return new cfar_middleware;
};
