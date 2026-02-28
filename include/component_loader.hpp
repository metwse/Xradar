/**
 * @file component_loader.hpp
 * @brief Dynamicly load components.
 */

#ifndef COMPONENT_LOADER_HPP
#define COMPONENT_LOADER_HPP


#include "components.hpp"

#include <cstddef>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>  // IWYU pragma: keep (enable doxygen to extract documentation)
#include <utility>


/** @brief Manages dynamically loaded component constructors. */
class component_loader {
public:
    /** @brief Loads components from the system. */
    component_loader() = default;

    /** @brief Loads components from given path. */
    component_loader(auto lib_dir_)
        : lib_dir { lib_dir_ } {}

    /** @brief Create a new producer component. */
    std::unique_ptr<component::base_producer> new_producer(
        std::string type,
        component::StateCallback state_callback,
        component::DataCallback data_callback
    ) {
        auto producer = dynamic_cast<component::base_producer *>(
            new_base(component::kind::producer, type)
        );

        producer->load_callbacks(state_callback, data_callback);

        return std::unique_ptr<component::base_producer>(producer);
    }

    /** @brief Create a new middleware component. */
    std::unique_ptr<component::base_middleware> new_middleware(
        std::string type,
        component::BackpressureCallback backpressure_callback,
        component::DataCallback data_callback
    ) {
        auto middleware = dynamic_cast<component::base_middleware *>(
            new_base(component::kind::middleware, type)
        );

        middleware->load_callbacks(backpressure_callback, data_callback);

        return std::unique_ptr<component::base_middleware>(middleware);
    }

    /** @brief Create a new consumer component. */
    std::unique_ptr<component::base_consumer> new_consumer(std::string type) {
        auto consumer = dynamic_cast<component::base_consumer *>(
            new_base(component::kind::consumer, type)
        );

        return std::unique_ptr<component::base_consumer>(consumer);
    }

private:
    /* RAII wrapper for dynamic component libraries. */
    using DlHandle = std::unique_ptr<void, std::function<int(void *)>>;

    /** @brief Dynamically loaded constructor function. */
    using ComponentConstructor = component::base_component *(*)();

    /**
     * @brief Concstucts component based on its class and type.
     *
     * @throw std::runtime_error dynamic loading is failed.
     */
    component::base_component *new_base(component::kind, std::string &type);

    std::filesystem::path lib_dir { "" };

    size_t last_type_id {};
    std::map<std::pair<component::kind, std::string /* type */>,
             size_t> type_map;

    std::map<size_t, DlHandle> libs;
    std::map<size_t, ComponentConstructor> constructors;
};


#endif
