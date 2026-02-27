#include "../src/config_grammar.hpp"
#include "../src/config_lex.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <sstream>


template<size_t Nm>
void test_lex(std::array<config_tk, Nm> token_ids,
              const char *input) {
    std::stringstream ss;
    ss << input;
    config_lex lex { ss };

    for (auto token_id : token_ids) {
        auto lex_token = lex.next();

        assert(lex_token.id == token_id && "token mismatch");

        seminfo_deleter(uint16_t(token_id), (void *)(&lex_token.seminfo));
    }
}


int main() {
    test_lex(std::array {
        TK_IDENT, TK_BOOL, TK_INT, TK_FLOAT, TK_LBRACE, TK_IDENT, TK_RBRACE, TK_SEMI,
    }, "test true 1 1.5 {test};");
}
