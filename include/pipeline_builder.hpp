#ifndef PIPELINE_BUILDER_HPP
#define PIPELINE_BUILDER_HPP


#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <string>
#include <vector>

class component_loader  /* defined in component_loader.hpp */;
namespace pipeline { class pipeline; }  /* defined in pipeline.hpp */;


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
    builder &connect(std::string_view name, std::string_view source_name);

    /** @brief Build the pipeline. */
    std::shared_ptr<pipeline> build(component_loader &);

private:
    /* component name/type strint to integer mapping */
    std::map<std::string, size_t> name_map;
    std::vector<std::string> name_map_rev;

    size_t last_name_id {};

    size_t get_name_id(std::string_view);
    const std::string &get_name(size_t i) const
        { return name_map_rev[i - 1]; }

    size_t producer_name_id {};
    size_t producer_type_id {};

    /* component id, type id */
    std::map<size_t, size_t> middleware;
    std::map<size_t, size_t> consumers;

    /* component id -> list of connected components to this source */
    std::map<size_t, std::vector<size_t>> routing_table;
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
