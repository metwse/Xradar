#ifndef CONFIG_GRAMMAR_HPP
#define CONFIG_GRAMMAR_HPP


#include <cstdio>

struct rdesc_grammar  /* defined in rdesc/grammar.h */;

enum config_tk {
    TK_NOTOKEN,
    TK_IDENT, TK_FLOAT, TK_INT, TK_STR, TK_BOOL,
    TK_LBRACE, TK_RBRACE, TK_SEMI
};

enum config_nt {
    NT_DIRECTIVE, NT_DIRECTIVES,
    NT_PARAMETER,
    NT_PARAMETER_LS, NT_PARAMETER_LS_REST,
    NT_OPTPARAMETER_LS
};


/* Lazily initialized grammar struct. */
rdesc_grammar *get_grammar();

/* Dump that grammar in BNF format. */
void dump_grammar(FILE *out);


#endif
