/**
 * @file config.hpp
 * @brief Configuration table.
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP


#include <cstddef>
#include <istream>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>


class config;
class parse_config;

/** @brief Table value type. */
using ConfigValue = std::variant<size_t,
                                 double,
                                 std::string,
                                 std::shared_ptr<config>>;

/** @brief Configuration table. */
class config {
public:
    /** @brief Dereference to get underlying data. */
    std::map<std::string, std::vector<ConfigValue>> &operator*()
        { return data; }

private:
    friend class ::parse_config;

    config() = default;

    config(auto data_)
        : data { data_ } {}

    std::map<std::string, std::vector<ConfigValue>> data;
};


/** @brief A class for parsing configuration table from text and reporting
 * parser status. */
class parse_config {
public:
    /** @brief Parse configuration table from input text stream. */
    parse_config(std::istream &);

    /** @brief Parsed config. */
    class config config;
};


#endif
