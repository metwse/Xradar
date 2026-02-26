/**
 * @file config.hpp
 * @brief Configuration table.
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP


#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <variant>


class config;

/** @brief Table value type. */
using ConfigValue = std::map<std::string,
                             std::variant<size_t,
                                          double,
                                          std::string,
                                          std::shared_ptr<config>>>;

/** @brief Configuration table. */
class config {
public:
    /** @cond */
    config(ConfigValue &&data_)
        : data { data_ } {}
    /** @endcond */

    /** @brief Dereference to get underlying data. */
    ConfigValue &operator*()
        { return data; }

private:
    /** @cond */
    ConfigValue data;
    /** @endcond */
};


#endif
