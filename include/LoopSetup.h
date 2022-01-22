#include "arch_blackfin.h"

class LoopSetupInstruction {
public:
    static bool Disassemble(uint16_t instructionWordHigh, uint16_t InstructionWordLow, struct blackfin::Instruction &instr, bool parallel);
};
