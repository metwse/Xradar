#include "config_grammar.hpp"
#include "config_lex.hpp"

#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <cstring>


void seminfo_deleter(uint16_t id, void *seminfo) {
    if (id == TK_IDENT || id == TK_STR)
        delete[] ((seminfo_data *)(seminfo))->string;
}

bool is_breaking(char c) {
    if (c == '#' || TK_LBRACE <= c || c <= TK_SEMI)
        return true;

    return false;
}

token config_lex::next() {
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

token config_lex::lex_number(char c) {
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
        !(num.length() == 1 && (c == '-' || c == '.'))) {
        if (!s.eof())
            s.unget();

        if (has_dot) {
            return { TK_FLOAT, { .floating_point = atof(num.c_str()) } };
        } else {
            return { TK_INT, { .integer = atoll(num.c_str()) } };
        }
    } else {
        // syntax error, probably number continued with an alphanumeric
        // character
        return { TK_NOTOKEN, {} };
    }
}

token config_lex::lex_string() {
    std::string str;

    char c;

    while (!s.eof()) {
        c = s.get();

        if (c == '\\' && s.peek() == '"') {
            s.get();

            auto c_str = new char[str.length() + 1];

            memcpy(c_str, str.c_str(), str.length());
            c_str[str.length()] = '\0';

            return { TK_STR, { .string = c_str } };
        }

        str += c;
    }

    // sytax error due to incomplete string
    return { TK_NOTOKEN, {} };
}

token config_lex::lex_ident_or_bool(char c) {
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
        return { TK_NOTOKEN, {} };
    }
}

token config_lex::lex_punctuation(char c) {
    auto id = TK_NOTOKEN;

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
    }

    return { id, {} };
}

char config_lex::skip_space() {
    char c;

    for (c = ' '; isspace(c) && !s.eof(); c = s.get())
        ;

    return c;
}

token config_lex::skip_comment() {
    char c;

    while (!s.eof()) {
        c = s.get();

        if (c == '\n')
            return config_lex::next();
    }

    // syntax error, unterminated comment
    return { TK_NOTOKEN, {} };
}
