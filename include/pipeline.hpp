#ifndef PIPELINE_HPP
#define PIPELINE_HPP


#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace component {
class base_producer  /* defined in components.hpp */;
class base_middleware  /* defined in components.hpp */;
class base_consumer  /* defined in components.hpp */;
}
namespace config {
class config  /* defined in config.hpp */;
}
namespace evloop { class evloop; }  /* defined in evloop.hpp */
namespace pipeline { class builder; }  /* defined in pipeline.hpp */


namespace pipeline {

/** @brief The signal processing pipeline. */
class pipeline {
public:
    /** @brief Start the processing pipeline. */
    void start();

    /** @brief Stop the processing pipeline. */
    void stop();

    /** @brief Send configuration to the component. */
    void config(config::config);

private:
    friend class buider;

    std::vector<std::string> name_map_rev;

    std::unique_ptr<component::base_producer> producer;
    size_t producer_id;
    std::map<size_t, std::unique_ptr<component::base_middleware>> middleware;
    std::map<size_t, std::unique_ptr<component::base_consumer>> consumers;

    std::shared_ptr<evloop::evloop> evloop;
};

}


#endif
