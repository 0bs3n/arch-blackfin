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

    int
    get_register_size(Register reg)
    {
        return reg_sizes[reg];
    }

    enum Register get_reg_for_reg_part(uint32_t reg)
    {
        return reg_part_to_reg[reg];
    }
}
