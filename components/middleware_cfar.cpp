#include "../include/components.hpp"
#include "../include/config.hpp"

#include <memory>


/** @brief Built-in CFAR middleware. */
class cfar_middleware : public component::base_middleware {
    void feed(std::shared_ptr<std::any>) override {};

    void configure(config::config) override {}
};


extern "C" cfar_middleware *constructor() {
    return new cfar_middleware;
};
