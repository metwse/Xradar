/**
 * @file components.hpp
 * @brief Producer, middleware, and consumer components.
 */

#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP


#include "config.hpp"

#include <any>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>


namespace component {

/** @brief Generic error reported by component types. */
struct error {
    /** @brief Implementation specific error code. */
    size_t code;
    /** @brief Human-readable error code. */
    std::string what;
};

/** @brief Signal source status. */
enum class producer_state {
    ready  /** @brief The producer is ready to start. */,
    starting  /** @brief Request to start producer has been sent. */,
    running  /** @brief Producer streaming data. */,
    stopping  /** @brief Request to stop producer has been sent. */,
    degraded  /** @brief A fatal error occured and cannot recover the source. */,
};


/** @brief Callback used in producers when they are reporting their state. */
using StateCallback = std::function<void(producer_state)>;

/** @brief Callback used when middleware cannot keep up with data stream. */
using BackpressureCallback = std::function<void(double)>;

/** @brief Data stream of producers or middleware. */
using DataCallback = std::function<void(std::unique_ptr<std::any>)>;


/** @brief Methods implemented by all components. */
class base_component {
public:
    virtual ~base_component() = default;

    /**
     * @brief Send component-specific configuration.
     *
     * The implementation can throw exceptions if provided configuration is
     * invalid.
     *
     * @throws component::error if configuration failed.
     */
    virtual void configure(config) = 0;
};


/** @brief Input source interface. */
class base_producer : public base_component {
public:
    /**
     * @brief Initialize producer source.
     *
     * A producer source shall initialize its connections during construction.
     * This call may be blocking, the event loop handle multhreading for this.
     *
     * @note Constructors can throw exceptions. Such errors will be catched and
     *       reported to the user.
     */
    base_producer(auto state_callback_, auto data_callback_)
        : state_callback { state_callback_ },
          data_callback { data_callback_ } {}

    /**
     * @brief Start the producer source.
     *
     * The implementation shall be in a valid state even if it throws error.
     *
     * @throws component::error if start failed.
     */
    virtual void start() = 0;

    /**
     * @brief Requests the source to stop.
     *
     * The implementation shall be in a valid state even if it throws error.
     *
     * @throws component::error if stop failed.
     */
    virtual void stop() = 0;

    /** @brief Hint the producer source about the consumer is overwhelmed. */
    virtual void hint_backpressure(double delay_t) = 0;

    /** @brief Data source reports its state using this callback. */
    StateCallback state_callback;

    /** @brief New data reported using this callback. */
    DataCallback data_callback;
};

/** @brief Signal processor middleware interface. */
class base_middleware : public base_component  {
public:
    /** @brief Middleware applies backpressure the producer. */
    base_middleware(auto backpressure_callback_, auto data_callback_)
        : backpressure_callback { backpressure_callback_ },
          data_callback { data_callback_ } {}

    /**
     * @brief Feed the middleware with input.
     *
     * This function blocks until the data accepted. Backpressure may be
     * triggered if this infilates the event loop queue.
     */
    virtual void feed(std::shared_ptr<std::any>) = 0;

    /** @brief Trigger backpressure on producer to slow it down. */
    BackpressureCallback backpressure_callback;

    /** @brief New data reported using this callback. */
    DataCallback data_callback;
};

/** @brief Processed signal consumer visualizer. */
class base_consumer : public base_component  {
public:
    /** @brief Send data to consumer, for visualizer. */
    virtual void send(std::shared_ptr<std::any>) = 0;
};

}


#endif
