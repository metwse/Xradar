#include "../src/config_grammar.hpp"
#include "../src/config_lex.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <sstream>


using enum config::tk;

template<size_t Nm>
void test_lex(std::array<config::tk, Nm> id_ls,
              std::initializer_list<config::seminfo> seminfo_ls,
              const char *input) {
    std::stringstream ss;
    ss << input;
    config::lex lex { ss };

    for (size_t i = 0; i < Nm; i++) {
        auto token_id = id_ls[i];
        auto seminfo = seminfo_ls.begin() + i;

        auto lex_token = lex.next();

        assert(lex_token.id == token_id);

        if (token_id == TK_IDENT || token_id == TK_STR) {
            assert(strcmp(lex_token.seminfo.string, seminfo->string) == 0);
        } else {
            assert(memcmp(&lex_token.seminfo, seminfo, sizeof(config::seminfo)) == 0);
        }

        config::seminfo_deleter(token_id, &lex_token.seminfo);
    }
}

template<size_t Nm>
void test_should_fail(std::array<config::tk, Nm> id_ls, const char *input) {
    std::stringstream ss;
    ss << input;
    config::lex lex { ss };

    for (size_t i = 0; i < Nm; i++) {
        auto token_id = id_ls[i];

        auto lex_token = lex.next();

        assert(lex_token.id == token_id);

        config::seminfo_deleter(token_id, &lex_token.seminfo);
    }

    bool failed = false;

    try {
        lex.next();
    } catch (config::lex_error &) {
        failed = true;
    };

    assert(failed);
}

int main() {
    test_lex(
        std::array {
            TK_IDENT, TK_BOOL, TK_BOOL, TK_INT, TK_FLOAT, TK_LBRACE, TK_IDENT,
            TK_RBRACE, TK_STR, TK_SEMI, TK_EOF
        },
        {
            { .string = "test" },
            { .boolean = true },
            { .boolean = false },
            { .integer = 1 },
            { .floating_point = 1.5 },
            {},
            { .string = "test" },
            {},
            { .string = "test \" \\" },
            {},
            {}
        },
        "test true false 1 1.5 {test} # this is a comment\n"
        "\"test \\\" \\\\\"; "
    );

    test_lex(
        std::array {
            TK_STR, TK_IDENT, TK_BOOL, TK_STR, TK_BOOL
        },
        {
            { .string = "\" \\ \\\\ \\\"\\" },
            { .string = "_0123" },
            { .boolean = true },
            { .string = "\\\\" },
            { .boolean = false }
        },
        "\"\\\" \\\\ \\\\\\\\ \\\\\\\"\\\\\" _0123 true \"\\\\\\\\\" false"
    );

    test_should_fail(std::array { TK_IDENT },
                     "valid_ident ident-invalid");

    test_should_fail(std::array { TK_INT, TK_FLOAT },
                     "1 -.5 -");
    test_should_fail(std::array { TK_INT, TK_FLOAT },
                     "2 -1. 1-0");
    test_should_fail(std::array { TK_INT, TK_FLOAT, TK_INT, TK_LBRACE, TK_RBRACE },
                     "2 -1. 1{} 2a");

    test_should_fail(std::array { TK_IDENT },
                     "test # no newline at the end");

    test_should_fail(std::array { TK_STR },
                     "\"a\"  \"unterminated string");

    test_should_fail(std::array { TK_LBRACE, TK_RBRACE, TK_SEMI },
                     "{};?");
}
