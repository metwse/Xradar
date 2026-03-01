#ifndef PIPELINE_HPP
#define PIPELINE_HPP


#include <cstddef>
#include <map>
#include <string>
#include <vector>

class component_manager  /* defined in component_manager.hpp */;


namespace pipeline {

class builder;

/** @brief The signal processing pipeline. */
class pipeline {
private:
    friend class builder;
};

/** @brief Builder for singal processing pipeline. */
class builder {
public:
    builder() = default;

    /**
     * @brief Set the producer of pipeline.
     *
     * @note Every pipeline must have only one producer.
     */
    builder &set_producer(std::string_view name, std::string_view kind);

    /** @brief Add a new middleware to the pipeline. */
    builder &add_middleware(std::string_view name, std::string_view kind);

    /** @brief Output consumer. */
    builder &add_consumer(std::string_view name, std::string_view kind);

    /** @brief Add a source to a consumer or middleware. */
    builder &connect(std::string_view name, std::string_view source_name);

    /** @brief Build the pipeline. */
    pipeline build(component_manager &);

private:
    /* component name/type strint to integer mapping */
    std::map<std::string, size_t> name_map;
    std::map<size_t, std::string> name_map_rev;

    size_t last_name_id {};

    size_t producer_name {};
    size_t producer_type {};

    /* component id, type id */
    std::map<size_t, size_t> middleware;
    std::map<size_t, size_t> consumers;

    /* producer id -> list of middleware that apply backpressure to producer */
    std::map<size_t, std::vector<size_t>> backpressure_table;

    /* component id -> list of connected components to this source */
    std::map<size_t, std::vector<size_t>> routing_table;
};

}


#endif
