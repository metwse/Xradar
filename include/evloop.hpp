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
#include <string>
#include <vector>


namespace evloop {

/** @brief Event loop frontend. */
class main {
public:
    /** @brief Create new producer. */
    void new_producer(std::string &, std::unique_ptr<component::base_producer>);

    /** @brief Create new middleware. */
    void new_middleware(std::string &, std::unique_ptr<component::base_middleware>);

    /** @brief Create new consumer. */
    void new_consumer(std::string &, std::unique_ptr<component::base_consumer>);

    /** @brief Configure component. */
    void config(std::string &, config &&);

    /** @brief Remove component. */
    void rm(std::string &);

    /** @brief Component status. */
    void stat(std::string &);

    /** @brief List of components. */
    std::vector<std::string &> ls();

    /** @brief Add source to the component. */
    void add_source(std::string &, std::vector<std::string>);

    /** @brief Start producer. */
    void start_producer(std::string &);

    /** @brief Stop producer. */
    void stop_producer(std::string &);

private:
    /* Numeric ids for component names. */
    std::map<std::string, size_t> name_map;

    std::map<size_t, std::vector<size_t>> routing_table;
    std::map<size_t, std::vector<size_t>> backpressure_table;

    std::map<size_t, std::unique_ptr<component::base_component>> components;
};

}


#endif
