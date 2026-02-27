#include "config_grammar.hpp"

#include "../external/include/rdesc/grammar.h"
#include "../external/include/rdesc/util.h"

#define PREFIX_TK(tk) TK_ ## tk
#define PREFIX_NT(nt) NT_ ## nt

#include "../external/include/rdesc/bnf_macros.h"

#include <cstdio>
#include <functional>
#include <memory>
#include <mutex>


#define TK_COUNT 10

#define NT_COUNT 6
#define NT_VARIANT_COUNT 6
#define NT_BODY_LENGTH 4


using namespace config;

static const rdesc_grammar_symbol
production_rules[NT_COUNT][NT_VARIANT_COUNT][NT_BODY_LENGTH] = {
    /* <directive> ::= */ r(
        TK(IDENT), NT(OPTPARAMETER_LS), TK(SEMI),
    alt TK(SEMI),
    ),

    /* <directives> ::= */ ropt(NT(DIRECTIVE), NT(DIRECTIVES)),

    /* <parameter> ::= */ r(
        TK(INT),
    alt TK(FLOAT),
    alt TK(STR),
    alt TK(BOOL),
    alt TK(LBRACE), NT(DIRECTIVES), TK(RBRACE),
    ),

    /* <parameter_ls> ::= */ r(
        NT(PARAMETER), NT(PARAMETER_LS_REST),
    ),

    /* <parameter_ls_rest> ::= */ ropt(NT(PARAMETER_LS), NT(PARAMETER_LS_REST)),

    /* <optparameter_ls> ::= */ ropt(NT(PARAMETER_LS))
};

static const char *const tk_names[TK_COUNT] = {
    "\0",
    "@ident", "@float", "@int", "@str", "@bool",
    "{", "}", ";",
    "EOF"
};

static const char *const nt_names[NT_COUNT] = {
    "directive", "directives",
    "parameter",
    "parameter_ls", "parameter_ls_rest",
    "opt_parameter_ls",
};

static rdesc_grammar global_grammar_;
static std::unique_ptr<rdesc_grammar, std::function<void(void *)>> global_grammar {
    &global_grammar_,
    [](void *) { rdesc_grammar_destroy(&global_grammar_); }
};

static bool global_grammar_initialized = false;
static std::mutex m_global_grammar;


rdesc_grammar *config::get_grammar() {
    std::lock_guard<std::mutex> guard { m_global_grammar };

    if (!global_grammar_initialized) {
        rdesc_grammar_init(&global_grammar_,
                           NT_COUNT, NT_VARIANT_COUNT, NT_BODY_LENGTH,
                           (const struct rdesc_grammar_symbol *) production_rules);

        global_grammar_initialized = true;
    }

    return &global_grammar_;
}

void config::dump_grammar(FILE *out) {
    rdesc_dump_bnf(out, get_grammar(), tk_names, nt_names);
}
