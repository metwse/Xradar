#include "../../include/components.hpp"
#include "../../include/pipeline_builder.hpp"
#include "../../include/pipeline.hpp"
#include "../../include/tpool.hpp"

#include "../lib/mock_component_loader.hpp"

#include <any>
#include <cassert>
#include <memory>


int finished = 0;

class generate_1 : public component::base_producer {
    void start() override {
        data_callback(int(1));
    }

    void stop() override {}  // GCOVR_EXCL_LINE
};

class add_1 : public component::base_middleware {
    std::any process(std::vector<std::any> in) override
        { return any_cast<int>(in[0]) + 1; }
};

class multiply_all : public component::base_middleware {
    std::any process(std::vector<std::any> in) override {
        int res = 1;

        for (auto &it : in)
            res *= std::any_cast<int>(it);

        return res;
    }
};

class add_all : public component::base_consumer {
    void send(std::vector<std::any> in) override {
        int res = 0;

        for (auto &it : in)
            res += std::any_cast<int>(it);

        assert(res == 8);

        finished++;
    }
};


int main() {
    auto tpool = tpool::tpool::create();

    mock_component_loader cl;
    pipeline::builder builder;

    cl.inject_component(component::kind::producer,
                        "generate_1",
                        []() { return (component::base_component *) new generate_1; });

    cl.inject_component(component::kind::middleware,
                        "add_1",
                        []() { return (component::base_component *) new add_1; });

    cl.inject_component(component::kind::middleware,
                        "multiply_all",
                        []() { return (component::base_component *) new multiply_all; });

    cl.inject_component(component::kind::consumer,
                        "add_all",
                        []() { return (component::base_component *) new add_all; });

    builder
        .set_producer("A", "generate_1")
        .set_state_callback([](component::producer_state) {})
        .add_middleware("B", "add_1")
        .add_middleware("C", "add_1")
        .add_middleware("D", "add_1")
        .add_middleware("E", "add_1")
        .add_middleware("F", "add_1")
        .add_middleware("G", "multiply_all")
        .add_consumer("H", "add_all")
        .add_middleware("I", "multiply_all")
        .add_middleware("J", "add_1")
        .add_consumer("K", "add_all")
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

    auto pipeline = builder.build(cl, tpool);

    pipeline->start();

    while (finished != 2)
        ;

    tpool->shutdown();
}
