#ifndef MOCK_COMPONENT_LOADER_HPP
#define MOCK_COMPONENT_LOADER_HPP


#include "../../include/components.hpp"
#include "../../include/component_loader.hpp"

#include <string_view>


class mock_component_loader : public component_loader {
public:
    using ComponentConstructor = component_loader::ComponentConstructor;

    void inject_component(component::kind kind,
                          std::string_view type,
                          ComponentConstructor constructor)
        { constructors[type_id(kind, type)] = constructor; }
};


#endif
