#include "../include/config.hpp"
#include "config_grammar.hpp"
#include "config_lex.hpp"

#include "../external/include/rdesc/cst_macros.h"
#include "../external/include/rdesc/rdesc.h"
#include "../external/include/rdesc/util.h"

#include <cctype>
#include <istream>
#include <new>


config::builder::builder() {
    if (rdesc_init(&p, get_grammar(),
                   sizeof(::config::seminfo),
                   seminfo_deleter))
        throw std::bad_alloc();  // GCOVR_EXCL_LINE

    rdesc_start(&p, NT_DIRECTIVE);
}

void config::builder::consume_tree(class config &c, struct rdesc_node *n) {
    if (rvariant(n) == 1)
        return;

    std::string key { *(char **) rseminfo(rchild(&p, n, 0)) };
    std::vector<ConfigValue> &value = c.data[key];

    auto optparameter_ls = rchild(&p, n, 1);

    if (rvariant(optparameter_ls) == 0  /* parameter_ls*/) {
        auto parameter_ls = rchild(&p, optparameter_ls, 0);

        while (true) {
            auto parameter = rchild(&p, parameter_ls, 0);

            if (rvariant(parameter) == 4  /* block */) {
                std::shared_ptr<::config::config> block {};

                consume_tree(*block, rchild(&p, n, 1));

                value.push_back(block);
            } else {
                void *any_seminfo = rseminfo(rchild(&p, parameter, 0));

                switch (rvariant(parameter)) {
                case 0: {  /* int */
                    size_t seminfo = *(size_t *) any_seminfo;
                    value.push_back(std::forward<size_t>(seminfo));
                    break;
                } case 1: {  /* float */
                    double seminfo = *(double *) any_seminfo;
                    value.push_back(std::forward<double>(seminfo));
                    break;
                } case 2: {  /* str */
                    char *seminfo = *(char **) any_seminfo;
                    value.push_back(std::forward<std::string>(seminfo));
                    break;
                } case 3: {  /* bool */
                    bool seminfo = *(bool *) any_seminfo;
                    value.push_back(std::forward<bool>(seminfo));
                    break;
                }
                }
            }

            auto parameter_ls_rest = rchild(&p, parameter_ls, 1);

            if (rvariant(parameter_ls_rest) == 1  /* E */)
                break;

            parameter_ls = rchild(&p, parameter_ls_rest, 0);
        }
    }
}

void config::builder::operator<<(std::istream &is) {
    lex lex { is };

    auto tk = lex.next();

    while (tk.id != TK_EOF) {
        auto res = rdesc_pump(&p, tk.id, &tk.seminfo);

        switch (res) {
        case RDESC_READY:
            consume_tree(config, rdesc_root(&p));

            rdesc_start(&p, NT_DIRECTIVE);
            break;

        case RDESC_NOMATCH:
            if (rdesc_reset(&p))
                    throw std::bad_alloc();  // GCOVR_EXCL_LINE

            rdesc_start(&p, NT_DIRECTIVE);
            break;

        case RDESC_CONTINUE:
            break;

        case RDESC_ENOMEM: throw std::bad_alloc(); // GCOVR_EXCL_LINE
        }

        tk = lex.next();
    }
}
