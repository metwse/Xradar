#include "../include/component_loader.hpp"
#include "../include/pipeline.hpp"
#include "../include/pipeline_builder.hpp"

#include <cstddef>
#include <memory>
#include <string_view>


std::shared_ptr<pipeline::pipeline> pipeline::builder::build(component_loader &cl) {
    auto p = pipeline::create();

    /* TODO: Make construction multithreaded
     * "constructors may be blocking" - stated in base component's
     * documentation. Pipeline build should handle this.
     * --------------------------------------------------------------------- */
    p->producer = cl.new_producer(get_name(producer_type_id),
                                  [](component::producer_state) {},
                                  [](std::unique_ptr<std::any>) {});
    p->producer_id = producer_name_id;

    for (auto &it : middleware)
        p->middleware[it.first] = cl.new_middleware(get_name(it.second),
                                                    [](double) {});

    for (auto &it : consumers)
        p->consumers[it.first] = cl.new_consumer(get_name(it.second));
    /* --------------------------------------------------------------------- */

    return p;
}

pipeline::builder &pipeline::builder::set_producer(std::string_view name,
                                                   std::string_view type) {
    producer_name_id = get_name_id(name);
    producer_type_id = get_name_id(type);

    return *this;
}

pipeline::builder &pipeline::builder::add_middleware(std::string_view name,
                                                     std::string_view type) {
    auto name_id = get_name_id(name);
    auto type_id = get_name_id(type);

    middleware[name_id] = type_id;

    return *this;
}

pipeline::builder &pipeline::builder::add_consumer(std::string_view name,
                                                   std::string_view type) {
    auto name_id = get_name_id(name);
    auto type_id = get_name_id(type);

    consumers[name_id] = type_id;

    return *this;
}

pipeline::builder &pipeline::builder::connect(std::string_view name,
                                              std::string_view source_name) {
    auto name_id = get_name_id(name);
    auto source_name_id = get_name_id(source_name);

    if (source_name_id != producer_name_id && !middleware.contains(source_name_id))
        throw build_error("source not found");

    if (!middleware.contains(name_id) && !consumers.contains(name_id))
        throw build_error("target not found");

    if (name_id == source_name_id)
        throw build_error("a middleware cannot feed itself");

    routing_table[source_name_id].push_back(name_id);

    return *this;
}

size_t pipeline::builder::get_name_id(std::string_view s) {
    size_t &id = name_map[std::string { s }];

    if (id == 0) {
        id = ++last_name_id;

        name_map_rev.push_back(std::string { s });
    }

    return id;
}
