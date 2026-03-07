#ifndef PIPELINE_HPP
#define PIPELINE_HPP


#include "components.hpp"
#include "config.hpp"

#include <any>
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace evloop { class evloop; }  /* defined in evloop.hpp */
namespace pipeline { class builder; }  /* defined in pipeline.hpp */
class execution;  /* internal: defined in pipeline.cpp */


namespace pipeline {

/** @brief The signal processing pipeline. */
class pipeline : public std::enable_shared_from_this<pipeline> {
private:
    struct token { explicit token() = default; };

public:
    pipeline(token, std::shared_ptr<evloop::evloop> evloop_)
        : evloop { evloop_ } {}

    /** @brief Start the processing pipeline. */
    void start()
        { producer->start(); }

    /** @brief Stop the processing pipeline. */
    void stop()
        { producer->stop(); }

    /** @brief Send configuration to the component. */
    void config(std::string_view name, config::config config) {
        size_t id = name_map[std::string { name }];

        if (producer_id == id)
            return producer->configure(config);

        if (middleware.contains(id))
            return middleware.at(id)->configure(config);

        if (consumers.contains(id))
            return consumers.at(id)->configure(config);
    }

private:
    friend class builder;
    friend class ::execution;

    static std::shared_ptr<pipeline> create(std::shared_ptr<evloop::evloop> evloop)
        { return std::make_shared<pipeline>(token {}, evloop); }

    void initiate(std::any data);

    std::map<std::string, size_t> name_map;

    std::unique_ptr<component::base_producer> producer;
    size_t producer_id;
    std::map<size_t, std::shared_ptr<component::base_middleware>> middleware;
    std::map<size_t, std::shared_ptr<component::base_consumer>> consumers;

    /* Parallelized topological order of middleware/consumer calls. */
    std::vector<std::vector<size_t>> order;

    std::map<size_t, std::vector<size_t>> input_table;

    std::shared_ptr<evloop::evloop> evloop;
};

}


#endif
