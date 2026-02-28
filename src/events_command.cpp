#include "../include/evloop.hpp"
#include "../include/events.hpp"

#include <memory>


using namespace events;


void new_component::operator()(std::shared_ptr<evloop::evloop>) {}

void configure::operator()(std::shared_ptr<evloop::evloop>) {}

void rm::operator()(std::shared_ptr<evloop::evloop>) {}

void add_source::operator()(std::shared_ptr<evloop::evloop>) {}

void start_producer::operator()(std::shared_ptr<evloop::evloop>) {}

void stop_producer::operator()(std::shared_ptr<evloop::evloop>) {}
