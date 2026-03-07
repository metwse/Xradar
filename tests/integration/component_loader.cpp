#include "../../include/component_loader.hpp"
#include "../../include/components.hpp"

#include <cassert>
#include <memory>
#include <stdexcept>


int main(int, const char **argv) {
    std::filesystem::path lib_dir { argv[0] };
    component_loader cl { lib_dir.parent_path() };

    auto producer = cl.new_producer("test",
                                    [](component::producer_state) {},  // GCOVR_EXCL_LINE
                                    [](std::any) {});
    assert(producer->kind() == component::kind::producer);
    producer->start();
    producer->stop();
    producer->hint_backpressure(0);

    auto middleware = cl.new_middleware("test",
                                        [](double) {});
    assert(middleware->kind() == component::kind::middleware);
    middleware->process({ 0 });

    auto consumer = cl.new_consumer("test");
    assert(consumer->kind() == component::kind::consumer);
    consumer->consume({ 0 });

    /* destruct previous consumer and allocate new one */
    consumer = cl.new_consumer("test");

    try {
        assert((cl.new_consumer("clearly-nonexistent-module"), "should throw"));
    } catch (std::runtime_error &) {}

    try {
        assert((cl.new_consumer("no-constructor"), "should throw"));
    } catch (std::runtime_error &) {}
}
