#include "../include/config.hpp"
#include "config_grammar.hpp"
#include "config_lex.hpp"

#include "../rdesc/include/cst_macros.h"
#include "../rdesc/include/rdesc.h"
#include "../rdesc/include/util.h"

#include <cctype>
#include <cstring>
#include <istream>
#include <memory>
#include <new>
#include <type_traits>
#include <variant>


void config::config::print(std::ostream &os, size_t indent_) const {
    std::string indent;

    for (size_t i = 0; i < indent_; i++)
        indent += " ";

    for (auto &it : data) {
        os << indent << it.first;

        for (auto &parameter : it.second) {
            std::visit(
                [&os, &indent, indent_](auto &arg) {
                    using T = std::decay_t<decltype(arg)>;

                    if constexpr (std::is_same_v<T, std::shared_ptr<config>>) {
                        os << "{\n";
                        arg->print(os, indent_ + 4);
                        os << indent << "}";
                    } else {
                        os << " " << arg;
                    }
                },
                parameter
            );
        }

        os << ";\n";
    }
}

namespace config {

std::ostream &operator<<(std::ostream &os, const config &c) {
    c.print(os, 0);

    return os;
}

}

config::builder::builder() {
    if (rdesc_init(&p, get_grammar(),
                   sizeof(::config::seminfo),
                   seminfo_deleter))
        throw std::bad_alloc();  // GCOVR_EXCL_LINE

    if (rdesc_start(&p, NT_DIRECTIVE))
        throw std::bad_alloc();  // GCOVR_EXCL_LINE
}

void config::builder::consume_tree(class config &c, struct rdesc_node *n) {
    if (ralt_id(n) == 1)
        return;


    char *key_ = NULL;
    memcpy(&key_, rseminfo(rchild(&p, n, 0)), sizeof(char *));

    std::string key { key_ };
    std::vector<ConfigValue> &value = c.data[key];

    auto optparameter_ls = rchild(&p, n, 1);

    if (ralt_id(optparameter_ls) == 0  /* parameter_ls*/) {
        auto parameter_ls = rchild(&p, optparameter_ls, 0);

        while (true) {
            auto parameter = rchild(&p, parameter_ls, 0);

            if (ralt_id(parameter) == 4  /* block */) {
                auto block = std::make_shared<class config>();

                auto directives = rchild(&p, parameter, 1);

                while (ralt_id(directives) == 0) {
                    auto directive = rchild(&p, directives, 0);

                    consume_tree(*block, directive);

                    directives = rchild(&p, directives, 1);
                }

                value.push_back(block);
            } else {
                void *any_seminfo = rseminfo(rchild(&p, parameter, 0));

                ConfigValue item;

                switch (ralt_id(parameter)) {
                case 0:  /* int */
                    item = *static_cast<size_t *>(any_seminfo);
                    break;
                case 1:  /* float */
                    item = *static_cast<double *>(any_seminfo);
                    break;
                case 2:  /* str */
                    item = *static_cast<char **>(any_seminfo);
                    break;
                case 3:  /* bool */
                    item = *static_cast<bool *>(any_seminfo);
                    break;
                }

                value.push_back(std::move(item));
            }

            auto parameter_ls_rest = rchild(&p, parameter_ls, 1);

            if (ralt_id(parameter_ls_rest) == 1  /* E */)
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

            rdesc_reset(&p);
            if (rdesc_start(&p, NT_DIRECTIVE))
                    throw std::bad_alloc();  // GCOVR_EXCL_LINE
            break;

        case RDESC_NOMATCH:
            rdesc_reset(&p);
            if (rdesc_start(&p, NT_DIRECTIVE))
                    throw std::bad_alloc();  // GCOVR_EXCL_LINE

            throw syntax_error("cannot parse input");

        case RDESC_CONTINUE:
            break;

        case RDESC_ENOMEM: throw std::bad_alloc(); // GCOVR_EXCL_LINE
        }

        tk = lex.next();
    }
}
