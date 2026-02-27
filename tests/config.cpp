#include "../include/config.hpp"

#include <sstream>


int main() {
    std::stringstream ss;

    ss << "key \"test\" true 123;";

    config::builder builder;

    builder << ss;
}
