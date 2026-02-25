/**
 * @file component_manager.hpp
 * @brief Dynamicly load components.
 */

#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP


#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>  // IWYU pragma: keep
#include <utility>

namespace component { class base_component; }  /* defined in components.hpp */


/** @brief Component class. */
enum class component_class {
    producer  /** @brief Producer kind of component. */,
    middleware  /** @brief Middleware kind of component. */,
    consumer /** @brief Consumer kind of component. */,
};


/** @brief Manages dynamically loaded component constructors. */
class component_manager {
public:
    component_manager() = default;

    /**
     * @brief Concstucts component based on its class and type.
     *
     * @throw std::runtime_error dynamic loading is failed.
     */
    std::unique_ptr<component::base_component> create(component_class,
                                                      std::string type);

private:
    /* RAII wrapper for dynamic component libraries. */
    using DlHandle = std::unique_ptr<void, std::function<void(void *)>>;

    /* Dynamically loaded constructor function. */
    using ComponentConstructor = std::function<void *()>;

    std::map<std::pair<component_manager, std::string /* type */>,
             size_t> type_map;

    std::map<size_t, DlHandle> libs;
    std::map<size_t, ComponentConstructor> constructors;
};


#endif
