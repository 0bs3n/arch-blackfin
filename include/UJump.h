#include "arch_blackfin.h"

class UJumpInstruction {
/* UJUMP
 +---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
 | 0 | 0 | 1 | 0 |.offset........................................|
 +---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
public:
    static bool Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
};
