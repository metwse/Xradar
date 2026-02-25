/**
 * @file components.hpp
 * @brief Input, middleware, and output components.
 */

#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP


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
enum class input_state {
    READY /** @brief The input is ready to start. */,
    STARTING /** @brief Request to start input has been sent. */,
    RUNNING /** @brief Input streaming data. */,
    STOPPING /** @brief Request to stop input has been sent. */,
    DEGRADED /** @brief A fatal error occured and cannot recover the source. */,
};


/** @brief Callback used in inputs when they are reporting their state. */
using StateCallback = std::function<void(input_state)>;

/** @brief Callback used when middleware cannot keep up with input stream. */
using BackpressureCallback = std::function<void(double)>;

/** @brief Data stream of inputs or middleware. */
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
     * @throws component::error
     */
    virtual void configure() = 0;
};


/** @brief Input source interface. */
class base_input : public base_component {
public:
    /**
     * @brief Initialize input source.
     *
     * An input source shall initialize its connections during construction.
     * This call may be blocking, the event loop handle multhreading for this.
     *
     * @note Constructors can throw exceptions. Such errors will be catched and
     *       reported to the user.
     */
    base_input(auto state_callback_, auto data_callback_)
        : state_callback { state_callback_ },
          data_callback { data_callback_ } {}

    virtual ~base_input() = default;

    /**
     * @brief Start the input source.
     *
     * The implementation shall be in a valid state even if it throws error.
     *
     * @throws component::error
     */
    virtual void start() = 0;

    /**
     * @brief Requests the source to stop.
     *
     * The implementation shall be in a valid state even if it throws error.
     *
     * @throws component::error
     */
    virtual void stop() = 0;

    /** @brief Hint the input source about the consumer is overwhelmed. */
    virtual void hint_backpressure(double delay_t) = 0;

    /** @brief Input source reports its state using this callback. */
    StateCallback state_callback;

    /** @brief New data reported using this callback. */
    DataCallback data_callback;
};

/** @brief Signal processor middleware interface. */
class base_middleware : public base_component  {
public:
    /** @brief Middleware applies backpressure the root input source. */
    base_middleware(auto backpressure_callback_, auto data_callback_)
        : backpressure_callback { backpressure_callback_ },
          data_callback { data_callback_ } {}

    virtual ~base_middleware() = default;

    /** @brief Feed the middleware with input. */
    virtual void feed(std::shared_ptr<std::any>) = 0;

    /** @brief Trigger backpressure on input to slow it down. */
    BackpressureCallback backpressure_callback;

    /** @brief New data reported using this callback. */
    DataCallback data_callback;
};

/** @brief Processed signal output visualizer. */
class base_output : public base_component  {
public:
    virtual ~base_output() = default;

    /** @brief Send data to output, for visualizer. */
    virtual void send(std::shared_ptr<std::any>) = 0;
};

}


#endif
