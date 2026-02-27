#include "../include/config.hpp"
#include "config_grammar.hpp"
#include "config_lex.hpp"

#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <cstring>


void config::seminfo_deleter(uint16_t id, void *seminfo) {
    if (id == TK_IDENT || id == TK_STR)
        delete[] ((::config::seminfo *)(seminfo))->string;
}

static bool is_breaking(char c) {
    if (c == '#' || c == '{' || c == '}' || c == ';' || isspace(c))
        return true;

    return false;
}

config::token config::lex::next() {
    char c = skip_space();

    if (isspace(c) || s.eof())
        return { TK_EOF, {} };

    if (c == '#')
        return skip_comment();

    if (isdigit(c) || c == '.' || c == '-')
        return lex_number(c);

    if (c == '"')
        return lex_string();

    if (isalpha(c) || c == '_')
        return lex_ident_or_bool(c);

    return lex_punctuation(c);
}

config::token config::lex::lex_number(char c) {
    std::string num;

    bool has_dot = false;

    while ((isdigit(c) ||
            (c == '.' && has_dot == false) ||
            (c == '-' && num.length() == 0)) &&
            !s.eof()) {
        num += c;

        if (c == '.')
            has_dot = true;

        c = s.get();
    }

    if ((s.eof() || is_breaking(c)) &&
        !(num.length() == 1 && (num == "-" || num == "."))) {
        if (!s.eof())
            s.unget();

        if (has_dot) {
            return { TK_FLOAT, { .floating_point = atof(num.c_str()) } };
        } else {
            return { TK_INT, { .integer = atoll(num.c_str()) } };
        }
    } else {
        throw lex_error("invalid number");
    }
}

config::token config::lex::lex_string() {
    std::string str;

    char c;

    while (!s.eof()) {
        c = s.get();

        if (c == '\\') {
            char next = s.peek();
            if (next == '\\' || next == '"') {
                str += s.get()  /* next */;

                continue;
            }
        }

        if (c == '"') {
            auto c_str = new char[str.length() + 1];

            memcpy(c_str, str.c_str(), str.length());
            c_str[str.length()] = '\0';

            return { TK_STR, { .string = c_str } };
        }

        str += c;
    }

    throw lex_error("unterminated string");
}


config::token config::lex::lex_ident_or_bool(char c) {
    std::string ident;

    while ((isalnum(c) || c == '_') && !s.eof()) {
        ident += c;

        c = s.get();
    }

    if (s.eof() || is_breaking(c)) {
        if (!s.eof())
            s.unget();

        if (ident == "true")
            return { TK_BOOL, { .boolean = true } };

        if (ident == "false")
            return { TK_BOOL, { .boolean = false } };

        auto c_ident = new char[ident.length() + 1];

        memcpy(c_ident, ident.c_str(), ident.length());
        c_ident[ident.length()] = '\0';

        return { TK_IDENT, { .string = c_ident } };
    } else {
        throw lex_error("invalid identifier");
    }
}

config::token config::lex::lex_punctuation(char c) {
    tk id;

    switch (c) {
    case '{':
        id = TK_LBRACE;
        break;
    case '}':
        id = TK_RBRACE;
        break;
    case ';':
        id = TK_SEMI;
        break;
    default:
        throw lex_error("unknown token");
    }

    return { id, {} };
}

char config::lex::skip_space() {
    char c;

    for (c = ' '; isspace(c) && !s.eof(); c = s.get())
        ;

    return c;
}

config::token config::lex::skip_comment() {
    char c;

    while (!s.eof()) {
        c = s.get();

        if (c == '\n')
            return lex::next();
    }

    throw lex_error("unterminated comment");
}
