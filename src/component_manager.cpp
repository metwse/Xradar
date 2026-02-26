#include "../include/component_manager.hpp"
#include "../include/components.hpp"

#include <filesystem>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>

#include <dlfcn.h>


ComponentConstructor component_manager::constructor_of(
    component::kind kind,
    std::string type
) {
    auto &id = type_map[std::make_pair(kind, type)];
    if (id == 0)
        id = ++last_type_id;

    void *dl_handle = NULL;
    ComponentConstructor constructor = NULL;

    if (!constructors.contains(id)) {
        std::filesystem::path file(lib_dir);

        file /= component::kind_to_string(kind);
        file += "_";
        file += type;
        file += ".so";

        dl_handle = dlopen(file.native().c_str(), RTLD_NOW);

        if (!dl_handle)
            throw std::runtime_error("Could not load the shared object!");
    }

    if (dl_handle) {
        constructor = ComponentConstructor(dlsym(dl_handle, "constructor"));

        if (!constructor) {
            dlclose(dl_handle);

            throw std::runtime_error("Could not load the shared object!");
        }

        libs.insert({
            id,
            std::unique_ptr<void, std::function<int(void *)>>(dl_handle, dlclose)
        });
    }

    if (constructor) {
        constructors.insert({ id, constructor });

        return constructor;
    }

     return constructors.at(id);
}
