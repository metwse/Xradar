/**
 * @file component_manager.hpp
 * @brief Dynamicly load components.
 */

#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP


#include <cstddef>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>  // IWYU pragma: keep (enable doxygen to extract documentation)
#include <utility>

namespace component {
    class base_component;  /* defined in components.hpp */
    enum class kind;  /* defined in components.hpp */
}


/** @brief Dynamically loaded constructor function. */
using ComponentConstructor = component::base_component (*)(...);

/** @brief Manages dynamically loaded component constructors. */
class component_manager {
public:
    /** @brief Component manager loads components from the system. */
    component_manager() = default;

    /** @brief Component manager loads components given path. */
    component_manager(auto lib_dir_)
        : lib_dir { lib_dir_ } {}

    /**
     * @brief Concstucts component based on its class and type.
     *
     * @throw std::runtime_error dynamic loading is failed.
     */
   ComponentConstructor constructor_of(component::kind, std::string type);

private:
    /* RAII wrapper for dynamic component libraries. */
    using DlHandle = std::unique_ptr<void, std::function<int(void *)>>;

    std::filesystem::path lib_dir { "" };

    size_t last_type_id {};
    std::map<std::pair<component::kind, std::string /* type */>,
             size_t> type_map;

    std::map<size_t, DlHandle> libs;
    std::map<size_t, ComponentConstructor> constructors;
};


#endif
