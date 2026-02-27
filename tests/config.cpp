#include "../include/config.hpp"

#include <iostream>
#include <sstream>


int main() {
    std::stringstream valid_input, invalid_input;
    config::builder builder;

    valid_input << R"(
        basic_types true false 123 123.123 "test";

        nested "test" {
            key1 false;
            key2;  # this is a comment
            key3 {
                test2;
            };
            key4 "test";
        };;;
    )";

    invalid_input << "invalid directive";

    builder << valid_input;

    try {
        builder << invalid_input;  /* ignored*/
    } catch (config::syntax_error &) { }

    auto config = *builder;

    std::cout << config;
}
