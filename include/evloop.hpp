/**
 * @file evloop.hpp
 * @brief Event loop.
 */

#ifndef EVLOOOP_HPP
#define EVLOOOP_HPP


#include "components.hpp"
#include "config.hpp"

#include <cstddef>
#include <map>
#include <memory>
#include <vector>


namespace evloop {

/** @brief Event loop frontend. */
class main {
public:
    /** @brief Create new producer. */
    size_t new_producer(std::unique_ptr<component::base_producer>);

    /** @brief Create new middleware. */
    size_t new_middleware(std::unique_ptr<component::base_middleware>);

    /** @brief Create new consumer. */
    size_t new_consumer(std::unique_ptr<component::base_consumer>);

    /** @brief Configure component. */
    void config(size_t id, config::config &&);

    /** @brief Remove component. */
    void rm(size_t id);

    /** @brief Component status. */
    void stat(size_t id);

    /** @brief Add source to the component. */
    void add_source(size_t id, std::vector<size_t> input_ids);

    /** @brief Start producer. */
    void start_producer(size_t id);

    /** @brief Stop producer. */
    void stop_producer(size_t id);

private:
    size_t last_id {};

    std::map<size_t, std::vector<size_t>> routing_table;
    std::map<size_t, std::vector<size_t>> backpressure_table;

    std::map<size_t, std::unique_ptr<component::base_component>> components;
};

}


#endif
