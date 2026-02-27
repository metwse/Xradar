#include "../include/config.hpp"
#include "config_grammar.hpp"
#include "config_lex.hpp"

#include "../external/include/rdesc/cst_macros.h"
#include "../external/include/rdesc/rdesc.h"

#include <cctype>
#include <istream>



config::builder::builder([[maybe_unused]] std::istream &s) {
    rdesc p;

    if (rdesc_init(&p, get_grammar(),
                   sizeof(::config::seminfo),
                   seminfo_deleter))
        throw std::bad_alloc();  // GCOVR_EXCL_LINE
}
