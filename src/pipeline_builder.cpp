#include "../include/component_loader.hpp"
#include "../include/pipeline.hpp"
#include "../include/pipeline_builder.hpp"

#include <cstddef>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>


std::shared_ptr<pipeline::pipeline> pipeline::builder::build(component_loader &cl) {
    auto p = pipeline::create();

    /* TODO: Make construction multithreaded
     * "constructors may be blocking" - stated in base component's
     * documentation. Pipeline build should handle this.
     * --------------------------------------------------------------------- */
    p->producer = cl.new_producer(get_string(producer_type_id),
                                  state_callback.value(),
                                  [](std::unique_ptr<std::any>) {});
    p->producer_id = producer_id;

    for (auto &it : middleware)
        p->middleware[it.first] = cl.new_middleware(get_string(it.second),
                                                    [](double) {});

    for (auto &it : consumers)
        p->consumers[it.first] = cl.new_consumer(get_string(it.second));
    /* --------------------------------------------------------------------- */

    /* See https://fahadsultan.com/csc223/datastructs/graphs_topo_kahn.html
     * Slightly modified for parallelization. */
    std::vector<std::vector<size_t>> sorted {};
    std::vector<size_t> queue { std::move(routing_table[producer_id]) };

    size_t total_sorted_size = 0;

    do {
        std::vector<size_t> next;
        std::vector<size_t> sorted_layer;

        for (auto &u : queue) {
            in_order_degree[u]--;

            if (in_order_degree[u] == 0) {
                sorted_layer.push_back(u);

                next.insert(next.end(),
                            routing_table[u].begin(),
                            routing_table[u].end());

                total_sorted_size++;
            }
        }

        sorted.push_back(std::move(sorted_layer));
        std::swap(queue, next);
    } while (!queue.empty());

    if (total_sorted_size != in_order_degree.size())
        throw build_error("cycle detected in pipeline");

    return p;
}

pipeline::builder &pipeline::builder::set_producer(std::string_view name,
                                                   std::string_view type) {
    producer_id = get_string_id(name);
    producer_type_id = get_string_id(type);

    return *this;
}

pipeline::builder &pipeline::builder::add_middleware(std::string_view name,
                                                     std::string_view type) {
    auto id = get_string_id(name);
    auto type_id = get_string_id(type);

    middleware[id] = type_id;

    return *this;
}

pipeline::builder &pipeline::builder::add_consumer(std::string_view name,
                                                   std::string_view type) {
    auto id = get_string_id(name);
    auto type_id = get_string_id(type);

    consumers[id] = type_id;

    return *this;
}

pipeline::builder &pipeline::builder::connect(std::string_view source_name,
                                              std::string_view name) {
    auto id = get_string_id(name);
    auto source_id = get_string_id(source_name);

    if (source_id != producer_id && !middleware.contains(source_id))
        throw build_error("source not found");

    if (!middleware.contains(id) && !consumers.contains(id))
        throw build_error("target not found");

    if (id == source_id)
        throw build_error("a middleware cannot feed itself");

    routing_table[source_id].push_back(id);
    input_table[id].push_back(source_id);

    /* In order degree is the total number of incoming edges to the node */
    in_order_degree[id]++;

    return *this;
}

pipeline::builder &pipeline::builder::set_state_callback(component::StateCallback cb) {
    state_callback = cb;

    return *this;
}

size_t pipeline::builder::get_string_id(std::string_view s) {
    size_t &id = string_id_map[std::string { s }];

    if (id == 0) {
        id = ++last_string_id;

        string_id_map_rev.push_back(std::string { s });
    }

    return id;
}
