#include "arch_blackfin.h"

class LinkageInstruction {
public:
    static bool Disassemble(uint16_t instructionWordHigh, u_int16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel);
};
