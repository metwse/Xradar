#include "../include/component_manager.hpp"
#include "../include/components.hpp"

#include <cassert>
#include <filesystem>


int main([[maybe_unused]] int argc, const char **argv) {
    std::filesystem::path lib_dir { argv[0] };

    lib_dir = lib_dir.parent_path();
    lib_dir /= "components/";

    component_manager cm { lib_dir };

    auto producer = cm.new_producer("test",
                                    [](component::producer_state) {},
                                    [](std::unique_ptr<std::any>) {});
    assert(producer->kind() == component::kind::producer);

    auto middleware = cm.new_middleware("test",
                                        [](double) {},
                                        [](std::unique_ptr<std::any>) {});
    assert(middleware->kind() == component::kind::middleware);

    auto consumer = cm.new_consumer("test");
    assert(consumer->kind() == component::kind::consumer);
}
