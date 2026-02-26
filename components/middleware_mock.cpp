#include "../include/components.hpp"
#include "../include/config.hpp"

#include <memory>


/** @brief Built-in mock middleware. */
class mock : public component::base_middleware {
    using base_middleware::base_middleware;

    void feed(std::shared_ptr<std::any>) override {};

    void configure(config) override {}
};


extern "C" mock *constructor(void *backpressure_callback, void *data_callback) {
    return new mock { backpressure_callback, data_callback };
};
