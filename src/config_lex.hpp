#ifndef CONFIG_LEX_HPP
#define CONFIG_LEX_HPP


#include "config_grammar.hpp"

#include <cctype>
#include <cstdint>
#include <istream>


union seminfo_data {
    double floating_point  /* TK_FLOAT */;
    int64_t integer  /* TK_INT */;
    bool boolean  /* TK_BOOL */;
    char *string  /* TK_IDENT TK_STR, null-terminated C string allocated by
                   * new[] */;
};

struct token {
    config_tk id;
    seminfo_data seminfo;
};


class config_lex {
public:
    config_lex(std::istream &s_)
        : s { s_ } {}

    /* Extract the next token. */
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


#endif
