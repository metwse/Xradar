#ifndef CONFIG_LEX_HPP
#define CONFIG_LEX_HPP


#include "config_grammar.hpp"

#include <cctype>
#include <cstdint>
#include <istream>


namespace config {

/** @brief Additional semantic information for a token. */
union seminfo {
    double floating_point  /** @brief TK_FLOAT */;
    int64_t integer  /** @brief TK_INT */;
    bool boolean  /** @brief TK_BOOL */;
    const char *string  /** @brief TK_IDENT TK_STR, null-terminated C string
                         * allocated by new[] */;
};

/** @brief Token (id + seminfo). */
struct token {
    tk id  /** @brief Token identifier. */;
    ::config::seminfo seminfo /** @brief Token seminfo. */;
};

/** @brief Lexer for configuration grammar. */
class lex {
public:
    /** @brief Construct lexer from input stream. */
    lex(std::istream &s_)
        : s { s_ } {}

    /** @brief Extract the next token. */
    token next();

private:
    std::istream &s;

    /* Extract ident or boolean token. true and false are reversed identifiers
     * and converted to token. */
    token lex_ident_or_bool(char c);

    /* Extract float or integer token. */
    token lex_number(char c);

    /* Extract string token. */
    token lex_string();

    /* One-character punctuation tokens. */
    token lex_punctuation(char c);

    /* Skip the rest of the line after #. */
    token skip_comment();

    /* Skip whitespace characters. */
    char skip_space();
};

void seminfo_deleter(uint16_t, void *);

}


#endif
