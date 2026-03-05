#ifndef PIPELINE_BUILDER_HPP
#define PIPELINE_BUILDER_HPP


#include "components.hpp"

#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <string>
#include <vector>

class component_loader  /* defined in component_loader.hpp */;
namespace pipeline { class pipeline; }  /* defined in pipeline.hpp */;
namespace evloop { class evloop; }  /* defined in evloop.hpp */;


namespace pipeline {

/** @brief Builder for singal processing pipeline. */
class builder {
public:
    builder() = default;

    /**
     * @brief Set the producer of pipeline.
     *
     * @note Every pipeline must have only one producer.
     */
    builder &set_producer(std::string_view name, std::string_view type);

    /** @brief Add a new middleware to the pipeline. */
    builder &add_middleware(std::string_view name, std::string_view type);

    /** @brief Output consumer. */
    builder &add_consumer(std::string_view name, std::string_view type);

    /** @brief Add a source to a consumer or middleware. */
    builder &connect(std::string_view source_name, std::string_view name);

    /** @brief Callack the producer reports its state to. */
    builder &set_state_callback(component::StateCallback cb);

    /**
     * @brief Build the pipeline.
     *
     * Calling this function more than once is not safe.
     */
    std::shared_ptr<pipeline> build(component_loader &,
                                    std::shared_ptr<evloop::evloop>);

private:
    /* component name/type strint to integer mapping */
    std::map<std::string, size_t> string_id_map;
    std::vector<std::string> string_id_map_rev;

    size_t last_string_id {};

    size_t get_string_id(std::string_view);
    const std::string &get_string(size_t i) const
        { return string_id_map_rev.at(i - 1); }

    size_t producer_id {};
    size_t producer_type_id {};

    /* component id, type id */
    std::map<size_t, size_t> middleware;
    std::map<size_t, size_t> consumers;

    /* component id -> list of connected components to this source
     * Used for topological ordering. */
    std::map<size_t, std::vector<size_t>> routing_table;

    /* List of middleware or producers connected to the middleware or consumer.
     * Used to pass data in order of connection creations. */
    std::map<size_t, std::vector<size_t>> input_table;

    /* In order degree of middleware and consumers. */
    std::map<size_t, size_t> in_order_degree;

    std::optional<component::StateCallback> state_callback;
};

/** @brief Pipeline build error. */
class build_error : std::exception {
public:
    /** @cond */
    build_error(const char *msg_)
        : msg { msg_ } {}

    const char *what() const noexcept override { return msg; }  // GCOVR_EXCL_LINE
    /** @endcond */

private:
    const char *msg;
};

}


#endif
