#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "components.hpp"
#include "evloop.hpp"

#include <cstddef>
#include <vector>
#include <memory>


namespace events {

/** @brief Scheduled event. */
class base_event {
public:
    /** @brief Operation. */
    virtual void operator()(std::shared_ptr<evloop::evloop>) = 0;
};


/** @brief Add a new producer. */
class new_producer : public base_event {
public:
    void operator()(std::shared_ptr<evloop::evloop>) override;

    /** @brief Producer to be added to evloop. */
    std::unique_ptr<component::base_producer> producer;
};

/** @brief Add a new middleware. */
class new_middleware : public base_event {
public:
    void operator()(std::shared_ptr<evloop::evloop>) override;

    /** @brief Middleware to be added to evloop. */
    std::unique_ptr<component::base_middleware> middleware;
};

/** @brief Add a new consumer. */
class new_consumer : public base_event {
public:
    void operator()(std::shared_ptr<evloop::evloop>) override;

    /** @brief Consumer to be added to evloop. */
    std::unique_ptr<component::base_consumer> consumer;
};

/** @brief Add sources to a component. */
class add_source : public base_event {
public:
    void operator()(std::shared_ptr<evloop::evloop>) override;

    /** @brief Middleware/consumer which receive input. */
    size_t id;

    /** @brief Ids of sources to feed with data. */
    std::vector<size_t> source_ids;
};

/** @brief Start the producer. */
class start_producer : public base_event {
public:
    void operator()(std::shared_ptr<evloop::evloop>) override;

    /** @brief Ids of the producer to be started. */
    size_t id;
};

/** @brief Stop the producer. */
class stop_producer : public base_event {
public:
    void operator()(std::shared_ptr<evloop::evloop>) override;

    /** @brief Ids of the producer to be stopped. */
    size_t id;
};

}


#endif
