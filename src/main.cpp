#include "../include/components.hpp"
#include "../include/component_loader.hpp"
#include "../include/pipeline.hpp"
#include "../include/pipeline_builder.hpp"
#include "../include/tpool.hpp"

#include <iostream>


int main(int, char *argv[]) {
    auto tpool = tpool::tpool::create();

    std::filesystem::path lib_dir { argv[0] };
    lib_dir = lib_dir.parent_path();
    lib_dir /= "components";
    component_loader cl { lib_dir };

    pipeline::builder builder;

    builder
        .set_producer("mrm", "mrm")
        .set_state_callback([](component::producer_state) {})
        .add_consumer("x11", "x11")
        .add_middleware("cfar", "cfar")
        .connect("mrm", "cfar")
        .connect("cfar", "x11");

    auto pipeline = builder.build(cl, tpool);

    pipeline->start();

    std::cout << "Press enter to exit" << std::endl;

    std::cin.ignore();
    tpool->shutdown();

    std::cout << "Exiting..." << std::endl;
}
