#include "../include/component_loader.hpp"
#include "../include/pipeline_builder.hpp"
#include "testutil.hpp"

#include <cassert>


int main(int, char *argv[]) {
    pipeline::builder builder;

    builder
        .set_producer("producer", "test")
        .add_middleware("middleware1", "test")
        .add_middleware("middleware2", "test")
        .add_middleware("middleware3", "test")
        .add_consumer("consumer", "test")
        .connect("middleware1", "producer")
        .connect("middleware2", "middleware1")
        .connect("middleware3", "middleware1")
        .connect("consumer", "middleware2")
        .connect("consumer", "middleware3");

    int failed = 0;

    try {
        builder.connect("clearnly-nonexistent-target", "producer");
    } catch (pipeline::build_error &) {
        failed++;
    }

    assert(failed == 1);

    try {
        builder.connect("consumer", "clearnly-nonexistent-producer");
    } catch (pipeline::build_error &) {
        failed++;
    }

    assert(failed == 2);

    try {
        builder.connect("middleware1", "middleware1");  /* circle */
    } catch (pipeline::build_error &) {
        failed++;
    }

    assert(failed == 3);

    TEST_COMPONENT_LOADER;

    builder.build(cl);
}
