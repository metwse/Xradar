#include "../include/component_manager.hpp"
#include "../include/components.hpp"

#include <cassert>
#include <filesystem>
#include <memory>
#include <stdexcept>


int main([[maybe_unused]] int argc, const char **argv) {
    std::filesystem::path lib_dir { argv[0] };

    lib_dir = lib_dir.parent_path();
    lib_dir /= "test-components";

    component_manager cm { lib_dir };

    auto producer = cm.new_producer("test",
                                    [](component::producer_state) {},  // GCOVR_EXCL_LINE
                                    [](std::unique_ptr<std::any>) {});
    assert(producer->kind() == component::kind::producer);
    producer->start();
    producer->stop();
    producer->hint_backpressure(0);

    auto middleware = cm.new_middleware("test",
                                        [](double) {},  // GCOVR_EXCL_LINE
                                        [](std::unique_ptr<std::any>) {});
    assert(middleware->kind() == component::kind::middleware);
    middleware->feed(std::make_shared<std::any>(0));

    auto consumer = cm.new_consumer("test");
    assert(consumer->kind() == component::kind::consumer);
    consumer->send(std::make_shared<std::any>(0));

    /* destruct previous consumer and allocate new one */
    consumer = cm.new_consumer("test");

    try {
        assert((cm.new_consumer("clearly-nonexistent-module"), "should throw"));
    } catch (std::runtime_error &) {}

    try {
        assert((cm.new_consumer("no-constructor"), "should throw"));
    } catch (std::runtime_error &) {}
}
