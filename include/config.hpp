/**
 * @file config.hpp
 * @brief Configuration table.
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP


#include "../external/include/rdesc/rdesc.h"

#include <cstddef>
#include <exception>
#include <istream>
#include <map>
#include <memory>
#include <ostream>
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
    config() = default;

    /** @brief Dereference to get  underlying data. */
    std::map<std::string, std::vector<ConfigValue>> &operator*()
        { return data; }


private:
    friend class builder;

    friend std::ostream &operator<<(std::ostream &os, const config &);

    void print(std::ostream &os, size_t indent) const;

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

    /** @brief Consume the matched config. */
    class config &&operator*()
        { return std::move(config); };

    /** @brief Feed the parser with new input. */
    void operator<<(std::istream &is);

private:
    rdesc p;

    void consume_tree(class config &, struct rdesc_node *);

    class config config;
};

/** @brief Parsing failed due to a lexer error. */
class lex_error : std::exception {
public:
    /** @cond */
    lex_error(const char *msg_)
        : msg { msg_ } {}

    const char *what() const noexcept override { return msg; }  // GCOVR_EXCL_LINE
    /** @endcond */

private:
    const char *msg;
};

/** @brief Parsing failed due to a syntax error. */
class syntax_error : std::exception {
public:
    /** @cond */
    syntax_error(const char *msg_)
        : msg { msg_ } {}

    const char *what() const noexcept override { return msg; }  // GCOVR_EXCL_LINE
    /** @endcond */

private:
    const char *msg;
};

}


#endif
