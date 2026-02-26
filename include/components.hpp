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

class component_manager  /* defined in component_manager.hpp */;


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

/** @brief Component kind. */
enum class kind {
    producer  /** @brief Producer kind of component. */,
    middleware  /** @brief Middleware kind of component. */,
    consumer /** @brief Consumer kind of component. */,
};

/** @brief Convert component kind enum to string. */
static inline std::string kind_to_string(enum kind kind) {
    switch (kind) {
    case kind::producer:
        return "producer";
    case kind::middleware:
        return "middleware";
    case kind::consumer:
        return "consumer";
    }

    throw "unreachable";  // GCOVR_EXCL_LINE
}


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

    /** @brief Component type. */
    virtual enum kind kind() const = 0;
};


/**
 * @brief Input source interface.
 *
 * A producer source shall initialize its connections during construction.
 * Constructor call may be blocking, the event loop handle multhreading for
 * this.
 */
class base_producer : public base_component {
public:
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
    virtual void hint_backpressure([[maybe_unused]] double delay_t) {};

    /** @returns component::kind::producer */
    enum kind kind() const override
        { return kind::producer; }

protected:
    /** @brief Data source reports its state using this callback. */
    StateCallback state_callback;

    /** @brief New data reported using this callback. */
    DataCallback data_callback;

private:
    friend class ::component_manager;

    void load_callbacks (auto state_callback_, auto data_callback_) {
        state_callback = state_callback_;
        data_callback = data_callback_;
    }
};

/** @brief Signal processor middleware interface. */
class base_middleware : public base_component  {
public:
    /**
     * @brief Feed the middleware with input.
     *
     * This function blocks until the data accepted. Backpressure may be
     * triggered if this infilates the event loop queue.
     */
    virtual void feed(std::shared_ptr<std::any>) = 0;

    /** @returns component::kind::middleware */
    enum kind kind() const override
        { return kind::middleware; }

protected:
    /** @brief Trigger backpressure on producer to slow it down. */
    BackpressureCallback backpressure_callback;

    /** @brief New data reported using this callback. */
    DataCallback data_callback;

private:
    friend class ::component_manager;

    void load_callbacks (auto backpressure_callback_, auto data_callback_) {
        backpressure_callback = backpressure_callback_;
        data_callback = data_callback_;
    }
};

/** @brief Processed signal consumer visualizer. */
class base_consumer : public base_component  {
public:
    /** @brief Send data to consumer, for visualizer. */
    virtual void send(std::shared_ptr<std::any>) = 0;

    /** @returns component::kind::consumer */
    enum kind kind() const override
        { return kind::consumer; }
};

}


#endif
