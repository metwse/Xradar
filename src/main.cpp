#include "../include/events.hpp"
#include "../include/evloop.hpp"

#include <memory>


int main() {
    auto evloop = evloop::evloop::create();

    evloop->push_event(
        std::make_unique<events::new_component>(component::kind::producer,
                                                "mrm")
    );

    evloop->push_event(
        std::make_unique<events::new_component>(component::kind::middleware,
                                                "cfar")
    );

    evloop->push_event(
        std::make_unique<events::new_component>(component::kind::consumer,
                                                "x11")
    );
}
