#include "../include/components.hpp"
#include "../include/config.hpp"

#include <memory>


/** @brief Built-in mock middleware. */
class mock_middleware : public component::base_middleware {
    void feed(std::shared_ptr<std::any>) override {};

    void configure(config) override {}
};


extern "C" mock_middleware *constructor() {
    return new mock_middleware;
};
