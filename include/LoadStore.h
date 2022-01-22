#include "arch_blackfin.h"

class LoadStoreInstruction {
public:
    static bool Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
};
