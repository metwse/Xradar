#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "components.hpp"
#include "config.hpp"
#include "evloop.hpp"

#include <cstddef>
#include <utility>
#include <vector>
#include <memory>


namespace events {

/** @brief Scheduled event. */
class base_event {
public:
    /** @brief Operation. */
    virtual void operator()(std::shared_ptr<evloop::evloop>) = 0;
};


/** @brief Add a new component. */
class new_component : public base_event {
public:
    /** @cond */
    new_component(component::kind kind_, std::string type_)
        : kind { kind_ }, type { type_ } {}
    /** @endcond */

    void operator()(std::shared_ptr<evloop::evloop>) override;

    /** @cond Kind of the new component. */
    component::kind kind;

    /** @cond Type of the new component. */
    std::string type;
};

/** @brief Configure the component. */
class configure : public base_event {
public:
    /** @cond */
    configure(auto config_)
        : config { std::move(config_) } {}
    /** @endcond */

    void operator()(std::shared_ptr<evloop::evloop>) override;

    /** @brief Configuration to be sent. */
    config::config config;
};

/** @brief Remove component. */
class rm : public base_event {
public:
    /** @cond */
    rm(auto id_)
        : id { id_ } {}
    /** @endcond */

    void operator()(std::shared_ptr<evloop::evloop>) override;

    /** @brief Component to be removed. */
    size_t id;
};

/** @brief Add sources to a component. */
class add_source : public base_event {
public:
    /** @cond */
    add_source(auto id_, auto source_ids_)
        : id { id_ }, source_ids { std::move(source_ids_) } {}
    /** @endcond */

    void operator()(std::shared_ptr<evloop::evloop>) override;

    /** @brief Middleware/consumer which receive input. */
    size_t id;

    /** @brief Ids of sources to feed with data. */
    std::vector<size_t> source_ids;
};

/** @brief Start the producer. */
class start_producer : public base_event {
public:
    /** @cond */
    start_producer(auto id_)
        : id { id_ } {}
    /** @endcond */

    void operator()(std::shared_ptr<evloop::evloop>) override;

    /** @brief Ids of the producer to be started. */
    size_t id;
};

/** @brief Stop the producer. */
class stop_producer : public base_event {
public:
    /** @cond */
    stop_producer(auto id_)
        : id { id_ } {}
    /** @endcond */

    void operator()(std::shared_ptr<evloop::evloop>) override;

    /** @brief Ids of the producer to be stopped. */
    size_t id;
};

}


#endif
