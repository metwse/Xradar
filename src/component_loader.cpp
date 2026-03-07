#include "../include/component_loader.hpp"
#include "../include/components.hpp"

#include <filesystem>
#include <functional>
#include <memory>
#include <stdexcept>

#include <dlfcn.h>


component::base_component *component_loader::new_base(component::kind kind,
                                                      std::string_view type) {
    auto id = type_id(kind, type);

    if (!constructors.contains(id)) {
        std::filesystem::path file(lib_dir);

        file /= component::kind_to_string(kind);
        file += "_";
        file += type;
        file += ".so";

        void *dl_handle = dlopen(file.native().c_str(), RTLD_NOW);

        if (!dl_handle)
            throw std::runtime_error("Could not load the shared object!");

        auto constructor = ComponentConstructor(dlsym(dl_handle, "constructor"));

        if (!constructor) {
            dlclose(dl_handle);

            throw std::runtime_error("Library does not contain 'constructor' symbol");
        }

        libs.insert({
            id,
            std::unique_ptr<void, std::function<int(void *)>>(dl_handle, dlclose)
        });

        constructors.insert({ id, constructor });

        return constructor();
    }

     return constructors.at(id)();
}
