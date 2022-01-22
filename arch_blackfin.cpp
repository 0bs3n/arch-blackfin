#include "include/arch_blackfin.h"

namespace blackfin {
    const char *
    get_register_name(Register reg) 
    {
        return reg_names[reg];
    }
    const char *
    get_operator_string(blackfin::Operator op) 
    {
        return operator_names[op];
    }

    const char *
    get_mnemonic_string(OpLiteral mnemonic)
    {
        return mnemonics[mnemonic];
    }
}
