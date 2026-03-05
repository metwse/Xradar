#include "../../include/evloop.hpp"
#include "../../include/component_loader.hpp"  // IWYU pragma: keep
#include "../../include/pipeline_builder.hpp"

#include "../lib/testutil.hpp"

#include <cassert>


int main(int, char *argv[]) {
    auto evloop = evloop::evloop::create();
    pipeline::builder builder;

    builder
        .set_producer("producer", "test")
        .set_state_callback([](component::producer_state) {})
        .add_middleware("middleware1", "test")
        .add_middleware("middleware2", "test")
        .add_middleware("middleware3", "test")
        .add_consumer("consumer", "test")
        .connect("producer", "middleware1")
        .connect("middleware1", "middleware2")
        .connect("middleware1", "middleware3")
        .connect("middleware2", "consumer")
        .connect("middleware3", "consumer");

    int failed = 0;

    try {
        builder.connect("producer", "clearnly-nonexistent-target");
    } catch (pipeline::build_error &) {
        failed++;
    }

    assert(failed == 1);

    try {
        builder.connect("clearnly-nonexistent-producer", "consumer");
    } catch (pipeline::build_error &) {
        failed++;
    }

    assert(failed == 2);

    try {
        builder.connect("middleware1", "middleware1");  /* cyclic */
    } catch (pipeline::build_error &) {
        failed++;
    }

    assert(failed == 3);

    TEST_COMPONENT_LOADER;

    builder.build(cl, evloop);

    pipeline::builder builder2;

    builder2
        .set_producer("p", "test")
        .set_state_callback([](component::producer_state) {})
        .add_middleware("m1", "test")
        .add_middleware("m2", "test")
        .connect("p", "m1")
        .connect("m1", "m2")
        .connect("m2", "m1");

    try {
        builder2.build(cl, evloop);  /* cyclic */
    } catch (pipeline::build_error &) {
        failed++;
    }

    assert(failed == 4);

    pipeline::builder builder3;

    builder3
        .set_producer("A", "test")
        .set_state_callback([](component::producer_state) {})
        .add_middleware("B", "test")
        .add_middleware("C", "test")
        .add_middleware("D", "test")
        .add_middleware("E", "test")
        .add_middleware("F", "test")
        .add_middleware("G", "test")
        .add_consumer("H", "test")
        .add_middleware("I", "test")
        .add_middleware("J", "test")
        .add_consumer("K", "test")
        .connect("A", "B")
        .connect("A", "C")
        .connect("A", "D")
        .connect("B", "E")
        .connect("B", "I")
        .connect("C", "F")
        .connect("C", "G")
        .connect("D", "G")
        .connect("D", "H")
        .connect("E", "I")
        .connect("F", "K")
        .connect("G", "J")
        .connect("I", "H")
        .connect("J", "K");

    builder3.build(cl, evloop);

    evloop->shutdown();
}
