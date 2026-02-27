/**
 * @file config.hpp
 * @brief Configuration table.
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP


#include "../external/include/rdesc/rdesc.h"

#include <cstddef>
#include <istream>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>


namespace config {

class config;
class builder;

/** @brief Table value type. */
using ConfigValue = std::variant<size_t,
                                 double,
                                 bool,
                                 std::string,
                                 std::shared_ptr<config>>;

/** @brief Configuration table. */
class config {
public:
    /** @brief Dereference to get underlying data. */
    std::map<std::string, std::vector<ConfigValue>> &operator*()
        { return data; }

private:
    friend class builder;

    config() = default;

    config(auto data_)
        : data { data_ } {}

    std::map<std::string, std::vector<ConfigValue>> data;
};


/** @brief A class for parsing configuration table from text and reporting
 * parser status. */
class builder {
public:
    /** @brief Parse configuration table from input text stream. */
    builder();

    ~builder()
        { rdesc_destroy(&p); }

    class config &&operator*() &&
        { return std::move(config); };

    void operator<<(std::istream &is);

private:
    rdesc p;

    void consume_tree(class config &, struct rdesc_node *);

    class config config;
};

}


#endif
